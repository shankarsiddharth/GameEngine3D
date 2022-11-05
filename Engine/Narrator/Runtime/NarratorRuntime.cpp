// NarratorRuntime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Core/StartNode.h"
#include "Core/EndNode.h"
#include "Core/DialogueNode.h"
#include "Core/DivertNode.h"
#include "Core/KnotNode.h"
#include "Core/ChoiceNode.h"
#include "Core/Graph.h"
#include "Utils/StringUtils.h"

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
		line = Narrator::Runtime::StringUtils::TrimCopy(line);
		if (!line.empty())
		{
			FileLines.emplace_back(line);
		}
	}
		

	for (const std::string& fileLine : FileLines)
	{
		std::cout << fileLine << std::endl;
	}

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
