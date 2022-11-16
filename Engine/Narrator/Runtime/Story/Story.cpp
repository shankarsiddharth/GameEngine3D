#include "Story.h"
#include "StorySyntax.h"
#include "ParseMetaData.h"

#include "../Core/StartNode.h"
#include "../Core/EndNode.h"
#include "../Core/Edge.h"
#include "../Core/DecisionNode.h"
#include "../Core/DialogueNode.h"
#include "../Core/DivertNode.h"
#include "../Core/KnotNode.h"
#include "../Core/ChoiceNode.h"
#include "../Core/Graph.h"
#include "../Utils/StringUtils.h"

#include "../JSON/Includes.h"
#include "../Core/NodeFactory.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <queue>
#include <iomanip>

Narrator::Runtime::Story::Story()
	: Narrator::Runtime::Graph(),
	m_StartNode(nullptr),
	m_EndNode(nullptr),
	m_CurrentNode(nullptr),
	m_LastDecisionNode(nullptr),
	m_LastChoiceNode(nullptr),
	m_IsAValidKnotSection(false),
	m_canRead(false)
{
	m_StartNode = new Narrator::Runtime::StartNode();
	AddToNodeMap(m_StartNode);
	m_EndNode = new Narrator::Runtime::EndNode();
	AddToNodeMap(m_EndNode);

	m_CurrentNode = m_StartNode;

}

Narrator::Runtime::Story::~Story()
{

}

bool Narrator::Runtime::Story::canRead()
{
	//TODO: #NarratorToDo Implementation
	return m_canRead;
}

std::string Narrator::Runtime::Story::Read()
{
	//TODO: #NarratorToDo Implementation
	std::string storyText;

	if (m_CurrentNode)
	{
		Narrator::Runtime::TNodeType currentNodeType = m_CurrentNode->GetType();
		Narrator::Runtime::TOutFlowType outFlowType = m_CurrentNode->GetOutFlowType();

		Narrator::Runtime::Node* nextNode = nullptr;

		switch (outFlowType)
		{
		default:
		case Narrator::Runtime::TOutFlowType::kNone:
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
		break;
		case Narrator::Runtime::TOutFlowType::kUniOutFlow:
		{
			Narrator::Runtime::UniOutFlowNode* outFlowNode = dynamic_cast<Narrator::Runtime::UniOutFlowNode*>(m_CurrentNode);
			if (outFlowNode)
			{
				nextNode = outFlowNode->GetNextNode();
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
		break;
		case Narrator::Runtime::TOutFlowType::kMultiOutFlow:
		{
			SetCanRead(false);
		}
		break;
		}

		switch (currentNodeType)
		{
		default:
		case Narrator::Runtime::TNodeType::kNodeBase:
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
		break;
		case Narrator::Runtime::TNodeType::kStart:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
		}
		break;
		case Narrator::Runtime::TNodeType::kEnd:
		{
			SetCanRead(false);
			m_CurrentNode = nextNode;
			storyText = "\n" + std::string("End of Story");
		}
		break;
		case Narrator::Runtime::TNodeType::kDialogue:
		{
			Narrator::Runtime::DialogueNode* dialogueNode = dynamic_cast<Narrator::Runtime::DialogueNode*>(m_CurrentNode);
			if (dialogueNode)
			{
				storyText = dialogueNode->GetText();
				m_CurrentNode = nextNode;
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
		break;
		case Narrator::Runtime::TNodeType::kDivert:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
			return "" + Read();
		}
		break;
		case Narrator::Runtime::TNodeType::kKnot:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
			return "" + Read();
		}
		break;
		case Narrator::Runtime::TNodeType::kChoice:
		{
			SetCanRead(true);
			m_CurrentNode = nextNode;
			return "" + Read();
		}
		break;
		case Narrator::Runtime::TNodeType::kDecision:
		{
			ReadChoices(m_CurrentNode);
			SetCanRead(false);
		}
		break;
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}

	return storyText;
}

std::vector<std::string> Narrator::Runtime::Story::GetChoices()
{
	//TODO: #NarratorToDo Implementation
	return m_CurrentChoices;
}

void Narrator::Runtime::Story::SelectChoice(uint32_t i_ChoiceIndex)
{
	//TODO: #NarratorToDo Implementation
	if (m_CurrentChoices.size() > 0)
	{
		if (i_ChoiceIndex >= 0 && i_ChoiceIndex < m_CurrentChoices.size())
		{
			Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(m_CurrentNode);
			if (decisionNode)
			{
				std::map<std::uint32_t, Narrator::Runtime::Node*> outFlowNodeMap = decisionNode->GetOutFlowNodeMap();
				std::uint32_t mapIndex = 0;
				for (std::map<std::uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = outFlowNodeMap.begin();
					mapIterator != outFlowNodeMap.end();
					mapIterator++, mapIndex++)
				{
					if (mapIndex == i_ChoiceIndex)
					{
						m_CurrentNode = mapIterator->second;
						if (m_CurrentNode)
						{
							m_CurrentChoices.clear();
							SetCanRead(true);
							return;
						}
						else
						{
							//TODO: #NarratorToDoAssert #RuntimeError
						}
					}
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
		else
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError		
	}
}

Narrator::Runtime::Story Narrator::Runtime::Story::Parse(const std::string& i_Path)
{
	//TODO: #NarratorToDo #Important Check if the End is missing it throws error

	std::map<std::size_t, std::string> FileLineMap;
	//Line Number and Corresponding ParseMetaData Map
	std::map<std::size_t, Narrator::Parser::ParseMetaData> ParseMetaDataMap;

	//Read a text file
	std::ifstream inFile(i_Path);
	std::string line;
	std::size_t lineNumber = 0;
	while (std::getline(inFile, line))
	{
		lineNumber++;
		std::istringstream iss(line);
		// 		std::cout << line << std::endl;
		std::string  lineTrim = Narrator::Runtime::StringUtils::TrimCopy(line);
		if (!lineTrim.empty())
		{
			FileLineMap.insert(std::pair<std::size_t, std::string>(lineNumber, lineTrim));
		}
	}

	//Create A Story Graph
	//Narrator::Runtime::Story* story = new Narrator::Runtime::Story();
	Narrator::Runtime::Story story;

	//Parse the Content
	for (std::map<std::size_t, std::string>::iterator fileLineMapIterator = FileLineMap.begin();
		fileLineMapIterator != FileLineMap.end();
		fileLineMapIterator++)
	{
		const std::size_t& fileLineNumber = fileLineMapIterator->first;
		const std::string& fileLineString = fileLineMapIterator->second;

		std::cout << fileLineMapIterator->first << " : " << fileLineString << std::endl;

		if (Narrator::Runtime::StringUtils::StartsWith(fileLineString, Narrator::Runtime::StorySyntax::KNOT_DECLARATION))
		{
			//Get the Knot Name
			std::string knotName;
			std::string tLine = fileLineString;
			Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::KNOT_DECLARATION);
			std::string nameToCheck = Narrator::Runtime::StringUtils::TrimCopy(tLine);
			if (nameToCheck.empty())
			{
				//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parser Error 
				//TODO: #NarratorToDo #NarratorMetaDataSample see:#NarratorMetaData
				std::string message = "Error Knot name cannot be empty";
				std::cout << message << std::endl;
				Narrator::Parser::ParseMetaData newParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
				ParseMetaDataMap.insert(std::pair<std::size_t, Narrator::Parser::ParseMetaData>(fileLineNumber, newParseMetaData));
			}
			else
			{
				if (Narrator::Runtime::StringUtils::IsValidKnotName(nameToCheck))
				{
					knotName = nameToCheck;
					//TODO: #NarratorToDo If KnotName already Present Throw Parsing Error
					if (story.HasKnotNode(knotName))
					{
						//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parser Error 
						std::cout << "Error Redefinition of knot name" << std::endl;
					}
					else
					{
						if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kKnot)
						{
							//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parsing Error
							std::cout << "At least One Line expected within knot" << std::endl;
						}

						if (!story.m_IsAValidKnotSection)
						{
							//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parsing Error
							std::cout << "Invalid Knot Flow. Missing DONE / END / Divert." << std::endl;
						}

						//Create Knot Node
						Narrator::Runtime::KnotNode* knotNode = new Narrator::Runtime::KnotNode();
						knotNode->SetName(knotName);
						story.AddNode(knotNode);
						story.AddToKnotNodeMap(knotName, knotNode);


						//Clear Knot Validity when creating a new Knot
						story.m_IsAValidKnotSection = false;
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parsing Error
					std::cout << "Error in knot name" << std::endl;
				}
			}
			story.ClearLastDecisionNode();
		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLineString, Narrator::Runtime::StorySyntax::CHOICE_DECLARATION))
		{
			if (story.GetLastDecisionNode() == nullptr)
			{
				if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert)
				{
					//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error
					std::cout << "Unreachable Choice after a divert" << std::endl;
				}
			}
			//Decision Point
			Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(story.CreateDecisionNode());

			if (decisionNode)
			{
				//Cache Decision Node for Knot Validity
				story.m_IsAValidKnotSection = true;

				std::string tLine = fileLineString;
				Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::CHOICE_DECLARATION);
				std::string choiceText = Narrator::Runtime::StringUtils::TrimCopy(tLine);
				if (choiceText.empty())
				{
					//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parser Error
					std::cout << "Choice Text cannot be empty" << std::endl;
				}
				else
				{
					std::uint32_t choiceIndex = decisionNode->GetDecisionPathCount();
					//Create Choice Node
					Narrator::Runtime::ChoiceNode* choiceNode = new Narrator::Runtime::ChoiceNode(choiceIndex, choiceText);
					story.AddNode(choiceNode);
					story.AddNodeLink(decisionNode, choiceNode);
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Parse Error
				std::cout << "Error while Creating Decision Node" << std::endl;
			}

		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLineString, Narrator::Runtime::StorySyntax::DIVERT_DECLARATION))
		{

			if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert
				|| story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kEnd)
			{
				//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error
				std::cout << "Unreachable Divert after a divert/END/DONE." << std::endl;
			}

			std::string tLine = fileLineString;
			//tLine = Narrator::Runtime::StringUtils::RemoveAllSpaces(tLine);

			tLine = Narrator::Runtime::StringUtils::TrimCopy(tLine);
			std::string possibleDivertName = tLine;
			Narrator::Runtime::StringUtils::RemoveAll(possibleDivertName, Narrator::Runtime::StorySyntax::DIVERT_DECLARATION);
			possibleDivertName = Narrator::Runtime::StringUtils::TrimCopy(possibleDivertName);

			if (possibleDivertName.empty())
			{
				//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parser Error
				std::cout << "The Divert Name should not be empty" << std::endl;
			}
			else
			{
				if (Narrator::Runtime::StringUtils::IsValidKnotName(possibleDivertName))
				{
					if (Narrator::Runtime::StringUtils::StartsWithIgnoreCase(possibleDivertName, Narrator::Runtime::StorySyntax::END_DECLARATION))
					{
						//Cache Decision Node for Knot Validity
						story.m_IsAValidKnotSection = true;

						//End Node
						story.LinkEndNode();
					}
					else if (Narrator::Runtime::StringUtils::StartsWithIgnoreCase(possibleDivertName, Narrator::Runtime::StorySyntax::DONE_DECLARATION))
					{
						//Cache Decision Node for Knot Validity
						story.m_IsAValidKnotSection = true;

						//Done Node is a divert node that redirects to the end node by default
						story.LinkEndNode();
					}
					else
					{
						Narrator::Runtime::DivertNode* divertNode = nullptr;

						//Get the Divert Name
						std::string divertName = possibleDivertName;

						//TODO: #NarratorToDo Check the Divert Node is already present
						if (story.HasDivertNode(divertName))
						{
							//Get the existing Divert Node
							Narrator::Runtime::Node* foundNode = story.GetDivertNode(divertName);
							divertNode = dynamic_cast<Narrator::Runtime::DivertNode*>(foundNode);
						}
						else
						{
							//Create Divert Node
							divertNode = new Narrator::Runtime::DivertNode();
							divertNode->SetTargetNodeName(divertName);
						}

						//Add the Node to the story
						if (divertNode)
						{
							//Cache Decision Node for Knot Validity
							story.m_IsAValidKnotSection = true;

							story.AddNode(divertNode);
							story.AddToDivertNodeMap(divertName, divertNode);
						}
						else
						{
							//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error : The Divert Node should not be nullptr
							std::cout << "The Divert Node should not be nullptr" << std::endl;
						}
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error
					std::cout << "Error - Not a Valid Divert Name" << std::endl;
				}
			}
		}
		else
		{
			if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert
				|| story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kEnd)
			{
				//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error
				std::cout << "Unreachable Dialogue after a divert / END / DONE" << std::endl;
			}

			//Create Dialogue Node
			Narrator::Runtime::DialogueNode* dialogueNode = new Narrator::Runtime::DialogueNode(fileLineString);
			story.AddNode(dialogueNode);
		}
	}


	{
		std::map<std::string, Narrator::Runtime::Node*> divertNodeMap = story.GetDivertNodeMap();
		//Check the Validity of the Divert Nodes
		for (std::map<std::string, Narrator::Runtime::Node*>::iterator divertNodeMapIterator = divertNodeMap.begin();
			divertNodeMapIterator != divertNodeMap.end();
			divertNodeMapIterator++)
		{
			Narrator::Runtime::DivertNode* divertNode = dynamic_cast<Narrator::Runtime::DivertNode*>(divertNodeMapIterator->second);
			if (divertNode)
			{
				std::string divertTargetName = divertNode->GetTargetNodeName();
				if (story.HasKnotNode(divertTargetName))
				{
					Narrator::Runtime::KnotNode* knotNode = dynamic_cast<Narrator::Runtime::KnotNode*>(story.GetKnotNode(divertTargetName));
					story.AddNodeLink(divertNode, knotNode);
					divertNode->SetTargetNode(knotNode);
				}
				else
				{
					//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error.
					//TODO: #NarratorToDo Add A DebugData Struct that has additional info for the node such as line number
					std::cout << "Divert Target not Found : " << divertTargetName << std::endl;
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert Throw Parse Error. This should not be null.
				std::cout << "DivertNode should not be null" << std::endl;
			}
		}
	}

	{
		std::map<std::string, Narrator::Runtime::Node*> knotNodeMap = story.GetKnotNodeMap();
		//Check the Reachability of the Knot Nodes
		for (std::map<std::string, Narrator::Runtime::Node*>::iterator knotNodeMapIterator = knotNodeMap.begin();
			knotNodeMapIterator != knotNodeMap.end();
			knotNodeMapIterator++)
		{
			Narrator::Runtime::KnotNode* knotNode = dynamic_cast<Narrator::Runtime::KnotNode*>(knotNodeMapIterator->second);
			if (knotNode)
			{
				const std::string& knotNodeName = knotNode->GetName();
				if (!story.HasDivertNode(knotNodeName))
				{
					//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error. / Warning
					std::cout << "Unreachable Knot Section Found : " << knotNodeName << std::endl;
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parse Error. This should not be null.
				std::cout << "KnotNode should not be null" << std::endl;
			}
		}
	}


	//Check for Empty Document
	Narrator::Runtime::StartNode* startNode = dynamic_cast<Narrator::Runtime::StartNode*>(story.m_StartNode);
	if (startNode)
	{
		Narrator::Runtime::Node* nextNode = startNode->GetNextNode();
		if (!nextNode)
		{
			//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Parse Error
			std::cout << "Empty Document is invalid." << std::endl;
		}
		else
		{
			//Check For loose knots
			if (!story.m_IsAValidKnotSection)
			{
				//TODO: #NarratorToDoAssert #NarratorToDoAssertParse Throw Parsing Error
				std::cout << "Invalid Knot Flow. Missing DONE / END / Divert." << std::endl;
			}
		}
	}


	//Traverse the Graph to validate the flow
	story.Traverse();

	//TODO: #NarratorToDo Only Export the JSON file if there are no parsing errors
// 	story.ToJSONFile("./TestScripts/story_test.json");

	return story;
}


void Narrator::Runtime::Story::Play()
{
	Narrator::Runtime::Node* currentNode = m_StartNode;
	while (currentNode->GetType() != TNodeType::kEnd)
	{

	}
	std::cout << "End of Story" << std::endl;
}

bool Narrator::Runtime::Story::ToJSONFile(const std::string& i_JSONFilePath)
{
	std::ofstream fout(i_JSONFilePath.c_str());
	if (fout.is_open())
	{
		nlohmann::json storyJSON;

		ToJSON(storyJSON);

		//TODO: #NarratorToDo Save the File
		fout << std::setw(4) << storyJSON << std::endl;

		fout.close();

		std::cout << "Write file complete" << std::endl;
	}
	else
	{
		std::cout << "Could not write to file" << std::endl;
	}

	return false;
}

bool Narrator::Runtime::Story::FromJSONFile(const std::string& i_JSONFilePath)
{
	//TODO: #NarratorToDo 

	std::ifstream fin(i_JSONFilePath.c_str());

	if (fin.is_open())
	{
		const nlohmann::json storyJSON = nlohmann::json::parse(fin);

		FromJSON(storyJSON);

		fin.close();

		std::cout << "Read file complete" << std::endl;
	}
	else
	{
		std::cout << "Could not read to file" << std::endl;
	}

	//TODO: #NarratorToDo Reset Story Read State
	m_CurrentNode = m_StartNode;
	m_CurrentChoices.clear();
	Read();

	return false;
}

void Narrator::Runtime::Story::ToJSON(nlohmann::json& jsonRoot)
{
	nlohmann::json nodeArray = nlohmann::json::array();

	//Nodes
	for (std::map<uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = m_NodeMap.begin();
		mapIterator != m_NodeMap.end();
		mapIterator++)
	{
		nlohmann::json nodeObject = nlohmann::json::object();
		mapIterator->second->ToJSON(nodeObject);
		nodeArray.emplace_back(nodeObject);
	}
	jsonRoot["nodes"] = nodeArray;

	nlohmann::json edgeArray = nlohmann::json::array();

	//Edges
	for (std::map<uint64_t, Narrator::Runtime::Edge*>::iterator mapIterator = m_EdgeMap.begin();
		mapIterator != m_EdgeMap.end();
		mapIterator++)
	{
		mapIterator;
	}
	jsonRoot["edges"] = edgeArray;

	nlohmann::json adjacencyListObject = nlohmann::json::object();

	//AdjacencyList
	for (std::map<uint32_t, std::vector<Narrator::Runtime::Node*>>::iterator mapIterator = m_AdjacencyListMap.begin();
		mapIterator != m_AdjacencyListMap.end();
		mapIterator++)
	{
		mapIterator;
	}
	jsonRoot["adjacency_list"] = adjacencyListObject;
}

void Narrator::Runtime::Story::FromJSON(const nlohmann::json& jsonRoot)
{
	if (jsonRoot.is_object())
	{
		const nlohmann::json nodeArray = jsonRoot["nodes"];
		if (nodeArray.is_array())
		{
			for (const nlohmann::json& nodeElement : nodeArray)
			{
				if (nodeElement.is_object())
				{
					if (nodeElement.contains("type"))
					{
						Narrator::Runtime::TNodeType nodeType = nodeElement["type"];
						//Start Node and End Node are created by default constructor
						if ((nodeType != TNodeType::kStart) && (nodeType != TNodeType::kEnd))
						{
							Narrator::Runtime::Node* newNode = NodeFactory::Create(nodeType);
							AddToNodeMap(newNode);
						}
					}
					else
					{
						//TODO: #NarratorToDoAssert #RuntimeError
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert #RuntimeError
				}
			}


		}
		else
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}

	if (jsonRoot.is_object())
	{
		const nlohmann::json nodeArray = jsonRoot["nodes"];
		if (nodeArray.is_array())
		{
			for (const nlohmann::json& nodeElement : nodeArray)
			{
				if (nodeElement.is_object())
				{
					if (nodeElement.contains("type"))
					{
						Narrator::Runtime::TNodeType nodeType = nodeElement["type"];
						switch (nodeType)
						{
						default:
						case Narrator::Runtime::TNodeType::kNodeBase:
						{
							//TODO: #NarratorToDoAssert #RuntimeError
						}
						break;
						case Narrator::Runtime::TNodeType::kStart:
						{
							m_StartNode->FromJSON(nodeElement, this);
							//TODO: #NarratorToDo Links for the Start Node is Missing
							//Create the node and fill up the link values
						}
						break;
						case Narrator::Runtime::TNodeType::kEnd:
						{
							m_EndNode->FromJSON(nodeElement, this);
							//TODO: #NarratorToDo Links for the Start Node is Missing
							//Create the node and fill up the link values
						}
						break;
						case Narrator::Runtime::TNodeType::kDialogue:
						case Narrator::Runtime::TNodeType::kDivert:
						case Narrator::Runtime::TNodeType::kKnot:
						case Narrator::Runtime::TNodeType::kChoice:
						case Narrator::Runtime::TNodeType::kDecision:
						{
							Narrator::Runtime::Node* currentNode = m_NodeMap[nodeElement["id"]];
							currentNode->FromJSON(nodeElement, this);
						}
						break;
						}
					}
					else
					{
						//TODO: #NarratorToDoAssert #RuntimeError
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert #RuntimeError
				}
			}
		}
		else
		{
			//TODO: #NarratorToDoAssert #RuntimeError
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}

	Traverse();

	std::cout << "Read Complete" << std::endl;
}

void Narrator::Runtime::Story::Traverse()
{
	BreadthFirstSearch();
}

void Narrator::Runtime::Story::BreadthFirstSearch()
{
	std::queue<Narrator::Runtime::Node*> nodeQueue;

	for (std::map<uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = m_NodeMap.begin();
		mapIterator != m_NodeMap.end();
		mapIterator++)
	{
		Narrator::Runtime::Node* currentNode = mapIterator->second;
		if (!currentNode->IsVisited())
		{
			nodeQueue.push(currentNode);
		}

		while (!nodeQueue.empty())
		{
			Narrator::Runtime::Node* frontNode = nodeQueue.front();
			frontNode->MarkAsVisited();

			//Print to standard output stream std::cout
			std::cout << frontNode->ToString() << std::endl;

			nodeQueue.pop();

			//TODO: #NarratorToDo Check the Previous Valid Nodes and add error checks for invalid flow

			if (frontNode)
			{
				std::vector<Narrator::Runtime::Node*> adjacentList = m_AdjacencyListMap[frontNode->GetID()];

				for (std::vector<Narrator::Runtime::Node*>::iterator adjacentListIterator = adjacentList.begin();
					adjacentListIterator != adjacentList.end();
					adjacentListIterator++)
				{
					Narrator::Runtime::Node* adjacentNode = *adjacentListIterator;
					if (!adjacentNode->IsVisited())
					{
						adjacentNode->MarkAsVisited();
						nodeQueue.push(adjacentNode);
					}
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert Throw Parse Error. This should not be null.
				std::cout << "FrontNode in the queue should not be null" << std::endl;
			}
		}
	}
}

void Narrator::Runtime::Story::AddNode(Narrator::Runtime::Node* i_NodeToAdd)
{
	AddEdge(m_CurrentNode, i_NodeToAdd);
	m_CurrentNode = i_NodeToAdd;
}

void Narrator::Runtime::Story::LinkEndNode()
{
	AddEdge(m_CurrentNode, m_EndNode);
	//TODO: #NarratorToDo #HighPriority If its an END Node is it necessary to make it as CurrentNode?
	m_CurrentNode = m_EndNode;
}

bool Narrator::Runtime::Story::HasDivertNode(const std::string& i_DivertName)
{
	return HasRedirectionNode(i_DivertName);
}

bool Narrator::Runtime::Story::HasKnotNode(const std::string& i_KnotName)
{
	return HasSubGraphStartNode(i_KnotName);
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetKnotNode(const std::string& i_KnotName)
{
	//TODO: #NarratorToDo Optimize this logic to just return the Node without checking / Return a nullptr if its not present
	if (HasDivertNode(i_KnotName))
	{
		return GetSubGraphStartNode(i_KnotName);
	}
	else
	{
		return nullptr;
	}
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetCurrentNode() const
{
	return m_CurrentNode;
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetDivertNode(const std::string& i_DivertName)
{
	//TODO: #NarratorToDo Optimize this logic to just return the Node without checking / Return a nullptr if its not present
	if (HasDivertNode(i_DivertName))
	{
		return GetRedirectionNode(i_DivertName);
	}
	else
	{
		return nullptr;
	}
}

void Narrator::Runtime::Story::AddToDivertNodeMap(const std::string& i_DivertName, Narrator::Runtime::Node* i_DivertNode)
{
	AddToRedirectionNodeMap(i_DivertName, i_DivertNode);
}

std::map<std::string, Narrator::Runtime::Node*> Narrator::Runtime::Story::GetDivertNodeMap()
{
	return m_RedirectionNodeMap;
}

void Narrator::Runtime::Story::AddToKnotNodeMap(const std::string& i_KnotName, Narrator::Runtime::Node* i_KnotNode)
{
	AddToSubGraphStartNodeMap(i_KnotName, i_KnotNode);
}

std::map<std::string, Narrator::Runtime::Node*> Narrator::Runtime::Story::GetKnotNodeMap()
{
	return m_SubGraphStartNodeMap;
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetLastDecisionNode() const
{
	return m_LastDecisionNode;
}

Narrator::Runtime::Node* Narrator::Runtime::Story::CreateDecisionNode()
{
	if (m_LastDecisionNode == nullptr)
	{
		Narrator::Runtime::DecisionNode* newDecisionNode = new Narrator::Runtime::DecisionNode();
		AddEdge(m_CurrentNode, newDecisionNode);

		m_CurrentNode = newDecisionNode;
		m_LastDecisionNode = newDecisionNode;
	}

	return m_LastDecisionNode;
}

void Narrator::Runtime::Story::ClearLastDecisionNode()
{
	m_LastDecisionNode = nullptr;
}

bool Narrator::Runtime::Story::GetCanRead() const
{
	return m_canRead;
}

void Narrator::Runtime::Story::SetCanRead(bool i_CanRead)
{
	m_canRead = i_CanRead;
}

void Narrator::Runtime::Story::ReadChoices(Narrator::Runtime::Node* i_CurrentDecisionNode)
{
	Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(i_CurrentDecisionNode);
	if (decisionNode)
	{
		m_CurrentChoices.clear();

		std::map<std::uint32_t, Narrator::Runtime::Node*> outFlowNodeMap = decisionNode->GetOutFlowNodeMap();

		for (std::map<std::uint32_t, Narrator::Runtime::Node*>::iterator mapIterator = outFlowNodeMap.begin();
			mapIterator != outFlowNodeMap.end();
			mapIterator++)
		{
			Narrator::Runtime::ChoiceNode* choiceNode = dynamic_cast<Narrator::Runtime::ChoiceNode*>(mapIterator->second);
			if (choiceNode)
			{
				std::string choiceText = choiceNode->GetChoiceText();
				m_CurrentChoices.emplace_back(choiceText);
			}
			else
			{
				//TODO: #NarratorToDoAssert #RuntimeError
			}
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert #RuntimeError
	}
}