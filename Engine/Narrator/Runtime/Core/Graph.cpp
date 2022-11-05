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
		//TODO: #NarratorToDoAssert Error Log / Assert
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
		std::uint32_t nodeID = i_NodeToAdd->GetID();
		if (m_NodeMap.find(nodeID) == m_NodeMap.end())
		{
			//Node Does not Exist
			m_NodeMap.insert(std::pair<uint32_t, Narrator::Runtime::Node*>(nodeID, i_NodeToAdd));
		}
		else
		{
			//TODO: #NarratorToDoLog Add Log that the nodeID is replaced
			m_NodeMap[nodeID] = i_NodeToAdd;
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert Error Log / Assert
	}
}

void Narrator::Runtime::Graph::AddToEdgeMap(Narrator::Runtime::Edge* i_EdgeToAdd)
{
	if (i_EdgeToAdd)
	{
		std::uint64_t edgeID = i_EdgeToAdd->GetID();
		if (m_EdgeMap.find(edgeID) == m_EdgeMap.end())
		{
			//Edge Does not Exist
			m_EdgeMap.insert(std::pair<uint64_t, Narrator::Runtime::Edge*>(i_EdgeToAdd->GetID(), i_EdgeToAdd));
		}
		else
		{
			//TODO: #NarratorToDoLog Add Log that the edgeID is replaced
			m_EdgeMap[edgeID] = i_EdgeToAdd;
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert Error Log / Assert
	}
}

void Narrator::Runtime::Graph::AddToAdjacencyListMap(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_AdjacentNode)
{
	if (i_SourceNode != nullptr && i_AdjacentNode != nullptr)
	{
		std::uint32_t sourceNodeID = i_SourceNode->GetID();
		std::map<uint32_t, std::vector<Narrator::Runtime::Node*>>::iterator mapIterator = m_AdjacencyListMap.find(sourceNodeID);
		if (mapIterator == m_AdjacencyListMap.end())
		{
			//AdjacencyList Not found for the Source Node
			
			//Create a new Adjacency List and add the adjacent Node
			std::vector<Narrator::Runtime::Node*> adjacencyList;
			adjacencyList.emplace_back(i_AdjacentNode);
			
			m_AdjacencyListMap.insert(std::pair<uint32_t, std::vector<Narrator::Runtime::Node*>>(sourceNodeID, adjacencyList));
		}
		else
		{
			mapIterator->second.emplace_back(i_AdjacentNode);
		}
	}
	else
	{
		//TODO: #NarratorToDoAssert Error Log / Assert
	}
}
