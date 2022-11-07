#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <string>

//Forward Declarations
namespace Narrator {
	namespace Runtime {
		class Node;
		class Edge;
	}
}

namespace Narrator
{
	namespace Runtime
	{
		class Graph
		{

		public:

			Graph();
			virtual ~Graph();

		protected:

			virtual void Traverse() = 0;

			void AddEdge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode);

			void AddToNodeMap(Narrator::Runtime::Node* i_NodeToAdd);
			void AddToEdgeMap(Narrator::Runtime::Edge* i_EdgeToAdd);
			void AddToAdjacencyListMap(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_AdjacentNode);
			void AddNodeLink(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_TargetNode);

			bool HasRedirectionNode(const std::string& i_RedirectionName);
			bool HasSubGraphStartNode(const std::string& i_SubGraphStartNodeName);
			Narrator::Runtime::Node* GetRedirectionNode(const std::string& i_RedirectionName);

			std::map<std::uint32_t, Narrator::Runtime::Node*> m_NodeMap;
			std::map<std::uint64_t, Narrator::Runtime::Edge*> m_EdgeMap;

			std::map<std::uint32_t, std::vector<Narrator::Runtime::Node*>> m_AdjacencyListMap;

			//TODO: #NarratorToDo Add to RedirectionNode Map and SubGraphStartNode Map
			//Map of All Redirection Nodes
			std::map<std::string, Narrator::Runtime::Node*> m_RedirectionNodeMap;
			//Map of All SubGraphs' Start Nodes(Section/Knots)
			std::map<std::string, Narrator::Runtime::Node*> m_SubGraphStartNodeMap;

		private:

		};
	}
}