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

			ChoiceNode(const std::uint32_t i_Index, const std::string& i_Text);
			~ChoiceNode();

			std::string ToString() override;

			std::uint32_t GetChoiceIndex();

		protected:

			std::uint32_t m_Index;
			std::string m_Text;

		};
	}
}