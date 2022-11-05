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
			
			Node(TNodeType i_Type = TNodeType::kNodeBase);
			virtual ~Node();

			TNodeType GetType() const;
			std::string GetName() const;
			std::uint32_t GetID() const;

		protected:

			static std::uint32_t m_CurrentIndex;

			std::uint32_t m_ID;
			std::string m_Name;
			TNodeType m_Type;

		private:

		};
	}
}