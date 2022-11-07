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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <queue>

Narrator::Runtime::Story::Story()
	: Narrator::Runtime::Graph(),
	m_StartNode(nullptr),
	m_EndNode(nullptr),
	m_CurrentNode(nullptr),
	m_LastDesicionNode(nullptr),
	m_LastChoiceNode(nullptr)
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
	return false;
}

std::string Narrator::Runtime::Story::Read()
{
	//TODO: #NarratorToDo Implementation
	return std::string("");
}

std::vector<Narrator::Runtime::ChoiceNode*> Narrator::Runtime::Story::GetChoices()
{
	//TODO: #NarratorToDo Implementation
	return {};
}

void Narrator::Runtime::Story::SelectChoice(uint32_t i_ChoiceIndex)
{

	//TODO: #NarratorToDo Implementation
}

Narrator::Runtime::Story Narrator::Runtime::Story::Parse(const std::string& i_Path)
{
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
				//TODO: #NarratorToDoAssert Throw Parser Error 
				std::string message = "Error Knot name cannot be empty";
				std::cout <<  message << std::endl;
				Narrator::Parser::ParseMetaData newParseMetaData(Narrator::Parser::TParseMessageType::kError, fileLineNumber, message);
				ParseMetaDataMap.insert(std::pair<std::size_t, Narrator::Parser::ParseMetaData>(fileLineNumber, newParseMetaData));
			}
			else
			{
				if (Narrator::Runtime::StringUtils::IsValidKnotName(nameToCheck))
				{
					knotName = nameToCheck;
					//TODO: #NarratorToDo If KnotName already Preset Throw Parsing Error
					if (story.HasKnotNode(knotName))
					{
						//TODO: #NarratorToDoAssert Throw Parser Error 
						std::cout << "Error Redefinition of knot name" << std::endl;
					}
					else
					{
						if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kKnot)
						{
							//TODO: #NarratorToDoAssert Throw Parsing Error
							std::cout << "At least One Line expected within knot" << std::endl;
						}
						//Create Knot Node
						Narrator::Runtime::KnotNode* knotNode = new Narrator::Runtime::KnotNode();
						knotNode->SetName(knotName);
						story.AddNode(knotNode);
						story.AddToKnotNodeMap(knotName, knotNode);
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert Throw Parsing Error
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
					//TODO: #NarratorToDoAssert Throw Parse Error
					std::cout << "Unreachable Choice after a divert" << std::endl;
				}
			}
			//Decision Point
			Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(story.CreateDecisionNode());

			if (decisionNode)
			{
				std::string tLine = fileLineString;
				Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::CHOICE_DECLARATION);
				std::string choiceText = Narrator::Runtime::StringUtils::TrimCopy(tLine);
				if (choiceText.empty())
				{
					//TODO: #NarratorToDoAssert Throw Parser Error
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
				//TODO: #NarratorToDoAssert Parse Error
				std::cout << "Error while Creating Decision Node" << std::endl;
			}

		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLineString, Narrator::Runtime::StorySyntax::DIVERT_DECLARATION))
		{

			if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert
				|| story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kEnd)
			{
				//TODO: #NarratorToDoAssert Throw Parse Error
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
				//TODO: #NarratorToDoAssert Throw Parser Error
				std::cout << "The Divert Name should not be empty" << std::endl;
			}
			else
			{
				if (Narrator::Runtime::StringUtils::IsValidKnotName(possibleDivertName))
				{
					if (Narrator::Runtime::StringUtils::StartsWithIgnoreCase(possibleDivertName, Narrator::Runtime::StorySyntax::END_DECLARATION))
					{
						//End Node
						story.LinkEndNode();
					}
					else if (Narrator::Runtime::StringUtils::StartsWith(possibleDivertName, Narrator::Runtime::StorySyntax::DONE_DECLARATION))
					{
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
							story.AddNode(divertNode);
							story.AddToDivertNodeMap(divertName, divertNode);
						}
						else
						{
							//TODO: #NarratorToDoAssert Throw Parse Error : The Divert Node should not be nullptr
							std::cout << "The Divert Node should not be nullptr" << std::endl;
						}
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert Throw Parse Error
					std::cout << "Error - Not a Valid Divert Name" << std::endl;
				}
			}			
		}
		else
		{
			if (story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kDivert
				|| story.GetCurrentNode()->GetType() == Narrator::Runtime::TNodeType::kEnd)
			{
				//TODO: #NarratorToDoAssert Throw Parse Error
				std::cout << "Unreachable Dialogue after a divert / END / DONE" << std::endl;
			}

			//Create Dialogue Node
			Narrator::Runtime::DialogueNode* dialogueNode = new Narrator::Runtime::DialogueNode(fileLineString);
			story.AddNode(dialogueNode);
		}
	}


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
			}
			else
			{
				//TODO: #NarratorToDoAssert Throw Parse Error.
				//TODO: #NarratorToDo Add A DebugData Struct that has additional info for the node such as line number
				std::cout << "Divert Target not Found" << std::endl;
			}
		}
		else
		{
			//TODO: #NarratorToDoAssert Throw Parse Error. This should not be null.
			std::cout << "DivertNode should not be null" << std::endl;
		}
	}

	//Traverse the Graph to validate the flow
	story.Traverse();

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
	return m_LastDesicionNode;
}

Narrator::Runtime::Node* Narrator::Runtime::Story::CreateDecisionNode()
{
	if (m_LastDesicionNode == nullptr)
	{
		Narrator::Runtime::DecisionNode* newDecisionNode = new Narrator::Runtime::DecisionNode();
		AddEdge(m_CurrentNode, newDecisionNode);

		m_CurrentNode = newDecisionNode;
		m_LastDesicionNode = newDecisionNode;
	}

	return m_LastDesicionNode;
}

void Narrator::Runtime::Story::ClearLastDecisionNode()
{
	m_LastDesicionNode = nullptr;
}