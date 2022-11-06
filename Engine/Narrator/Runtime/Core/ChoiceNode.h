#pragma once

#include "UniInFlowNode.h"
#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class ChoiceNode : public UniInFlowNode, public UniOutFlowNode
		{

		public:

			ChoiceNode(); 
			~ChoiceNode();

			std::uint32_t GetChoiceIndex();

		protected:
			std::uint32_t m_Index;
		};
	}
}