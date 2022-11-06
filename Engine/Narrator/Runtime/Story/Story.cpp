#include "Story.h"
#include "../Core/StartNode.h"
#include "../Core/EndNode.h"
#include "../Core/Edge.h"
#include "../Core/DecisionNode.h"

#include <queue>
#include <iostream>

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

void Narrator::Runtime::Story::Read()
{
	//TODO: #NarratorToDo Implementation
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

void Narrator::Runtime::Story::AddNode(Narrator::Runtime::Node* i_NodeToAdd)
{
/*	
	AddToNodeMap(i_NodeToAdd);
	Narrator::Runtime::Edge* newEdge = new Narrator::Runtime::Edge(m_CurrentNode, i_NodeToAdd);
	AddToEdgeMap(newEdge);
	AddToAdjacencyListMap(m_CurrentNode, i_NodeToAdd);
	AddNodeLink(m_CurrentNode, i_NodeToAdd);
*/

	AddEdge(m_CurrentNode, i_NodeToAdd);
	m_CurrentNode = i_NodeToAdd;
}

void Narrator::Runtime::Story::LinkEndNode()
{
/*
	Narrator::Runtime::Edge* newEdge = new Narrator::Runtime::Edge(m_CurrentNode, m_EndNode);
	AddToEdgeMap(newEdge);
	AddToAdjacencyListMap(m_CurrentNode, m_EndNode);
	AddNodeLink(m_CurrentNode, m_EndNode);
*/
	
	AddEdge(m_CurrentNode, m_EndNode);
}

bool Narrator::Runtime::Story::HasDivertNode(const std::string& i_DivertName)
{
	return HasRedirectionNode(i_DivertName);
}

bool Narrator::Runtime::Story::HasKnotNode(const std::string& i_KnotName)
{
	return HasSubGraphStartNode(i_KnotName);
}

Narrator::Runtime::Node* Narrator::Runtime::Story::GetDivertNode(const std::string& i_DivertName)
{
	//TODO: #NarratorToDo Optimize this logic to just return the Node without checking
	if (HasDivertNode(i_DivertName))
	{
		return GetRedirectionNode(i_DivertName);
	}
	else
	{
		return nullptr;
	}
}

Narrator::Runtime::Node* Narrator::Runtime::Story::CreateDecisionNode()
{
	if (m_LastDesicionNode == nullptr)
	{
		Narrator::Runtime::DecisionNode* newDecisionNode = new Narrator::Runtime::DecisionNode();
		AddToNodeMap(newDecisionNode);
		Narrator::Runtime::Edge* newEdge = new Narrator::Runtime::Edge(m_CurrentNode, newDecisionNode);
		AddToEdgeMap(newEdge);
		AddToAdjacencyListMap(m_CurrentNode, newDecisionNode);
		AddNodeLink(m_CurrentNode, newDecisionNode);
		m_CurrentNode = newDecisionNode;
		m_LastDesicionNode = newDecisionNode;
	}
	
	return m_LastDesicionNode;
}

void Narrator::Runtime::Story::ClearLastDecisionNode()
{
	m_LastDesicionNode = nullptr;
}

bool Narrator::Runtime::Story::HasAValidDecisionNode()
{
	if (m_LastDesicionNode != nullptr)
	{
		return true;
	}

	return false;
}

bool Narrator::Runtime::Story::HasAValidChoiceNode()
{
	if (m_LastChoiceNode != nullptr)
	{
		return true;
	}
	
	return false;
}

void Narrator::Runtime::Story::SetLastChoiceNode(Narrator::Runtime::Node* i_ChoiceNode)
{
	if (i_ChoiceNode->GetType() != TNodeType::kChoice)
	{
		//TODO: #NarratorToDoAssert Throw Parsing Error
	}
	else
	{
		m_LastChoiceNode = i_ChoiceNode;
	}
}

void Narrator::Runtime::Story::ClearLastChoiceNode()
{
	m_LastChoiceNode = nullptr;
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
	}
}

