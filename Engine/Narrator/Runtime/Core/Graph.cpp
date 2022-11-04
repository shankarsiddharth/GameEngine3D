#include "Graph.h"
#include "Node.h"
#include "Edge.h"

Narrator::Runtime::Graph::Graph()
{

}

Narrator::Runtime::Graph::~Graph()
{

}

void Narrator::Runtime::Graph::AddEdge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode)
{
	if (i_SourceNode == nullptr)
	{
		//TODO: #NarratorToDo Error Log / Assert
		return;
	}

	Narrator::Runtime::Edge* newEdge = new Narrator::Runtime::Edge(i_SourceNode, i_DestinationNode);
	AddToNodeMap(i_SourceNode);
	AddToNodeMap(i_DestinationNode);
	AddToEdgeMap(newEdge);
	AddToAdjacencyListMap(i_SourceNode, i_DestinationNode);
}

void Narrator::Runtime::Graph::AddToNodeMap(Narrator::Runtime::Node* i_NodeToAdd)
{
	if (i_NodeToAdd)
	{
		if (m_NodeMap.find(i_NodeToAdd->GetID()) == m_NodeMap.end())
		{
			//Node Does not Exist
			m_NodeMap.insert(std::pair<uint32_t, Narrator::Runtime::Node*>(i_NodeToAdd->GetID(), i_NodeToAdd));
		}
	}
	else
	{
		//TODO: #NarratorToDo Error Log / Assert
	}
}

void Narrator::Runtime::Graph::AddToEdgeMap(Narrator::Runtime::Edge* i_EdgeToAdd)
{
	if (i_EdgeToAdd)
	{
		if (m_EdgeMap.find(i_EdgeToAdd->GetID()) == m_EdgeMap.end())
		{
			//Edge Does not Exist
			m_EdgeMap.insert(std::pair<uint64_t, Narrator::Runtime::Edge*>(i_EdgeToAdd->GetID(), i_EdgeToAdd));
		}
	}
	else
	{
		//TODO: #NarratorToDo Error Log / Assert
	}
}

void Narrator::Runtime::Graph::AddToAdjacencyListMap(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_AdjacentNode)
{
	if (i_SourceNode != nullptr && i_AdjacentNode != nullptr)
	{
		std::map<uint32_t, std::vector<Narrator::Runtime::Node*>>::iterator mapIterator = m_AdjacencyListMap.find(i_SourceNode->GetID());
		if (mapIterator == m_AdjacencyListMap.end())
		{
			std::vector<Narrator::Runtime::Node*> adjacencyList;
			adjacencyList.emplace_back(i_AdjacentNode);
			//AdjacencyList Not found for the Source Node
			m_AdjacencyListMap.insert(std::pair<uint32_t, std::vector<Narrator::Runtime::Node*>>(i_SourceNode->GetID(), adjacencyList));
		}
		else
		{
			mapIterator->second.emplace_back(i_AdjacentNode);
		}
	}
	else
	{
		//TODO: #NarratorToDo Error Log / Assert
	}
}
