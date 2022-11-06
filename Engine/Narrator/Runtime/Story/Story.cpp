#include "Story.h"
#include "../Core/StartNode.h"
#include "../Core/EndNode.h"
#include "../Core/Edge.h"
#include "../Core/DecisionNode.h"

Narrator::Runtime::Story::Story()
	: Narrator::Runtime::Graph(),
	m_StartNode(nullptr),
	m_EndNode(nullptr),
	m_CurrentNode(nullptr),
	m_LastDesicionNode(nullptr)
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
	return std::vector<Narrator::Runtime::ChoiceNode*>();
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

void Narrator::Runtime::Story::Traverse()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

