// NarratorRuntime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Core/StartNode.h"
#include "Core/EndNode.h"
#include "Core/DialogueNode.h"
#include "Core/DivertNode.h"
#include "Core/KnotNode.h"
#include "Core/ChoiceNode.h"
#include "Core/Graph.h"

int main()
{

/*
	Narrator::Runtime::StartNode startNode;
	std::cout << std::to_string(startNode.GetID()) << std::endl;

	Narrator::Runtime::DialogueNode startNode1("dialogue");
	std::cout << std::to_string(startNode1.GetID()) << std::endl;
*/

	//Read a text file
	std::ifstream inFile("story.txt");
	std::string line;
	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		std::cout << line << std::endl;
	}

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

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
