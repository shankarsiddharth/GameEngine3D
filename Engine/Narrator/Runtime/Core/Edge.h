#pragma once

#include <cstdint>
#include <string>

//Forward Declarations
namespace Narrator {
	namespace Runtime {
		class Node;
	}
}

namespace Narrator
{
	namespace Runtime
	{
		//Edge Class
		class Edge
		{

		public:

			static size_t GetTotalEdgeCount();
			static size_t GetMaxEdgeCount();

			Edge();
			Edge(Narrator::Runtime::Node* i_SourceNode, Narrator::Runtime::Node* i_DestinationNode);
			virtual ~Edge();

			std::string GetName() const;
			std::uint64_t GetID() const;
		protected:

			static std::uint64_t m_CurrentIndex;

			std::uint64_t m_ID;
			std::string m_Name;

			Narrator::Runtime::Node* m_SourceNode;
			Narrator::Runtime::Node* m_DestinationNode;

		private:

		};
	}
}