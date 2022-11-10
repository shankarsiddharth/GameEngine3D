#pragma once

#include "MultiInFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class EndNode : public MultiInFlowNode
		{

		public:

			EndNode();
			~EndNode();

			void ToJSON(nlohmann::json& nodeObject) override;

		};
	}
}