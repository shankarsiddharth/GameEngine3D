#pragma once

#include "NodeType.h"

#include <string>

namespace Narrator
{
	namespace Runtime
	{
		class Node
		{
		public:

			static size_t GetTotalNodeCount();
			static	size_t GetMaxNodeCount();
			
			Node(TNodeType i_NodeType = TNodeType::kNone);
			~Node();

			TNodeType GetType();
			std::string GetName();

		protected:	
			static uint32_t m_CurrentNodeIndex;

			uint32_t m_NodeID;
			std::string m_NodeName;
			TNodeType m_NodeType;

		private:

		};
	}
}