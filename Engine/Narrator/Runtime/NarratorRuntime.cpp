// NarratorRuntime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include "Core/StartNode.h"
#include "Core/EndNode.h"
#include "Core/DialogueNode.h"
#include "Core/DivertNode.h"
#include "Core/KnotNode.h"
#include "Core/ChoiceNode.h"
#include "Core/Graph.h"
#include "Utils/StringUtils.h"
#include "Story/Story.h"
#include "Story/StorySyntax.h"
#include "Core/DecisionNode.h"


int main()
{
	std::vector<std::string> FileLines;

	//Read a text file
	std::ifstream inFile("story.txt");
	std::string line;
	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
// 		std::cout << line << std::endl;
		std::string  lineTrim = Narrator::Runtime::StringUtils::TrimCopy(line);
		if (!lineTrim.empty())
		{
			FileLines.emplace_back(lineTrim);
		}
	}
	
	//Create A Story Graph
	Narrator::Runtime::Story* story = new Narrator::Runtime::Story();

	//Parse the Content
	for (const std::string& fileLine : FileLines)
	{
		std::cout << fileLine << std::endl;
		if (Narrator::Runtime::StringUtils::StartsWith(fileLine, Narrator::Runtime::StorySyntax::KNOT_DECLARATION))
		{ 
			//Get the Knot Name
			std::string knotName;
			std::string tLine = fileLine;
			Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::KNOT_DECLARATION);
			std::string nameToCheck = Narrator::Runtime::StringUtils::TrimCopy(tLine);
			if (Narrator::Runtime::StringUtils::IsValidKnotName(nameToCheck))
			{
				knotName = nameToCheck;
				//TODO: #NarratorToDo If KnotName already Preset Throw Parsing Error
				if (story->HasKnotNode(knotName))
				{
					//TODO: #NarratorToDoAssert Throw Parser Error 
					std::cout << "Error Redefinition of knot name" << std::endl;
				}
				else
				{
					//Create Knot Node
					Narrator::Runtime::KnotNode* knotNode = new Narrator::Runtime::KnotNode();
					knotNode->SetName(knotName);
					story->AddNode(knotNode);
				}
			}
			else
			{
				//TODO: #NarratorToDoAssert Throw Parsing Error
				std::cout << "Error in knot name" << std::endl;
			}
			story->ClearLastDecisionNode();
		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLine, Narrator::Runtime::StorySyntax::CHOICE_DECLARATION))
		{
			//Decision Point
			Narrator::Runtime::DecisionNode* decisionNode = dynamic_cast<Narrator::Runtime::DecisionNode*>(story->CreateDecisionNode());
			
			if (decisionNode)
			{
				std::string tLine = fileLine;
				Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::CHOICE_DECLARATION);
				std::string choiceText = Narrator::Runtime::StringUtils::TrimCopy(tLine);
				std::uint32_t choiceIndex = decisionNode->GetDecisionPathCount();
				//Create Choice Node
				Narrator::Runtime::ChoiceNode* choiceNode = new Narrator::Runtime::ChoiceNode(choiceIndex, choiceText);
				story->AddNode(choiceNode);
			}
			else
			{
				//TODO: #NarratorToDoAssert Parse Error
				std::cout << "Error while Creating Decision Node" << std::endl;
			}

		}
		else if (Narrator::Runtime::StringUtils::StartsWithIgnoreCase(fileLine, Narrator::Runtime::StorySyntax::END_DECLARATION))
		{
			//End Node
			story->LinkEndNode();
		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLine, Narrator::Runtime::StorySyntax::DONE_DECLARATION))
		{
			//Done Node is a divert node that redirects to the end node by default
			story->LinkEndNode();
		}
		else if (Narrator::Runtime::StringUtils::StartsWith(fileLine, Narrator::Runtime::StorySyntax::DIVERT_DECLARATION))
		{
			std::string tLine = fileLine;
			tLine = Narrator::Runtime::StringUtils::RemoveAllSpaces(tLine);

			if (Narrator::Runtime::StringUtils::StartsWithIgnoreCase(tLine, Narrator::Runtime::StorySyntax::END_DECLARATION))
			{
				//End Node
				story->LinkEndNode();
			}
			else if (Narrator::Runtime::StringUtils::StartsWith(tLine, Narrator::Runtime::StorySyntax::DONE_DECLARATION))
			{
				//Done Node is a divert node that redirects to the end node by default
				story->LinkEndNode();
			}
			else
			{
				//Get the Divert Name
				std::string divertName;
				std::string tLine = fileLine;
				Narrator::Runtime::StringUtils::RemoveAll(tLine, Narrator::Runtime::StorySyntax::DIVERT_DECLARATION);
				std::string divertTargetName = Narrator::Runtime::StringUtils::TrimCopy(tLine);
				if (Narrator::Runtime::StringUtils::IsValidKnotName(divertTargetName))
				{
					divertName = divertTargetName;
					//TODO: #NarratorToDo Check the Divert Node is already present
					if (story->HasDivertNode(divertName))
					{
						Narrator::Runtime::Node* foundNode = story->GetDivertNode(divertName);
						Narrator::Runtime::DivertNode* divertNode = dynamic_cast<Narrator::Runtime::DivertNode*>(foundNode);
						if (divertNode)
						{
							story->AddNode(divertNode);
						}
						else
						{
							//TODO: #NarratorToDoAssert Throw Parse Error : The Divert Node should not be nullptr
						}
					}
					else
					{
						//Create Divert Node
						Narrator::Runtime::DivertNode* divertNode = new Narrator::Runtime::DivertNode();
						divertNode->SetTargetNodeName(divertName);
						story->AddNode(divertNode);
					}
				}
				else
				{
					//TODO: #NarratorToDoAssert Throw Parsing Error
					std::cout << "Error in divert name" << std::endl;
				}
			}			
		}
		else
		{
			//Create Dialogue Node
			Narrator::Runtime::DialogueNode* dialogueNode = new Narrator::Runtime::DialogueNode(fileLine);
			story->AddNode(dialogueNode);
		}
	}

	//Traverse the Graph to validate the flow


/*
	Narrator::Runtime::StartNode startNode;
	std::cout << std::to_string(startNode.GetID()) << std::endl;

	Narrator::Runtime::DialogueNode startNode1("dialogue");
	std::cout << std::to_string(startNode1.GetID()) << std::endl;
*/

/*
	Narrator::Runtime::StartNode startNode;

	Narrator::Runtime::DialogueNode dialogueNode1("Hi");
	Narrator::Runtime::DivertNode divertNode1;

	Narrator::Runtime::KnotNode knotNode;
	Narrator::Runtime::DialogueNode dialogueNode2("Knot Start");
	
	Narrator::Runtime::ChoiceNode choiceNode1;
	Narrator::Runtime::DialogueNode dialogueNode3("Choice 1 Dialogue");
	Narrator::Runtime::DivertNode divertNode2;
	
	Narrator::Runtime::ChoiceNode choiceNode2;
	Narrator::Runtime::DialogueNode dialogueNode4("Choice 2 Dialogue");
	Narrator::Runtime::DivertNode divertNode3;

	Narrator::Runtime::ChoiceNode choiceNode3;
	Narrator::Runtime::DialogueNode dialogueNode5("Choice 3 Dialogue");
	Narrator::Runtime::DivertNode divertNode4;

	Narrator::Runtime::ChoiceNode choiceNode4;
	Narrator::Runtime::DialogueNode dialogueNode6("Choice 4 Dialogue");
	Narrator::Runtime::DivertNode divertNode5;

	Narrator::Runtime::EndNode endNode;

	Narrator::Runtime::Graph newGraph;

	newGraph.AddEdge(&startNode, &dialogueNode1);
	newGraph.AddEdge(&dialogueNode1, &divertNode1);
	newGraph.AddEdge(&divertNode1, &knotNode);
	newGraph.AddEdge(&knotNode, &dialogueNode2);

	std::cout << std::to_string(Narrator::Runtime::Node::GetMaxNodeCount()) << std::endl;
	std::cout << std::to_string(Narrator::Runtime::Node::GetTotalNodeCount()) << std::endl;
*/

}
