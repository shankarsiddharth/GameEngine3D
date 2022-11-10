#pragma once

#include "UniInFlowNode.h"
#include "MultiOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class DecisionNode : public UniInFlowNode, public MultiOutFlowNode
		{

		public:

			DecisionNode();
			~DecisionNode();

			std::string ToString() override;

			std::uint32_t GetDecisionPathCount();


			void ToJSON(nlohmann::json& nodeObject) override;

		};
	}
}