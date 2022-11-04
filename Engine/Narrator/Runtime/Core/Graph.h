#pragma once

#include <cstdint>
#include <map>
#include <vector>

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

			void AddEdge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode);

		protected:	

			void AddToNodeMap(Narrator::Runtime::Node* i_NodeToAdd);
			void AddToEdgeMap(Narrator::Runtime::Edge* i_EdgeToAdd);
			void AddToAdjacencyListMap(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_AdjacentNode);

			std::map<std::uint32_t, Narrator::Runtime::Node*> m_NodeMap;
			std::map<std::uint64_t, Narrator::Runtime::Edge*> m_EdgeMap;

			std::map<std::uint32_t, std::vector<Narrator::Runtime::Node*>> m_AdjacencyListMap;

		private:

		};
	}
}