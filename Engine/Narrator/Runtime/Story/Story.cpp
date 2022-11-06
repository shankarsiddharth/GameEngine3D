#include "Story.h"
#include "../Core/StartNode.h"
#include "../Core/EndNode.h"
#include "../Core/Edge.h"

Narrator::Runtime::Story::Story()
	: Narrator::Runtime::Graph(),
	m_StartNode(nullptr),
	m_EndNode(nullptr)
{
	m_StartNode = new Narrator::Runtime::StartNode();
	m_EndNode = new Narrator::Runtime::EndNode();
	
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
	AddToNodeMap(i_NodeToAdd);
	Narrator::Runtime::Edge* newEdge = new Narrator::Runtime::Edge(m_CurrentNode, i_NodeToAdd);
	AddToEdgeMap(newEdge);
	AddToAdjacencyListMap(m_CurrentNode, i_NodeToAdd);
}

