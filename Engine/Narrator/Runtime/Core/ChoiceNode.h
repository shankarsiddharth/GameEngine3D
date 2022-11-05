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
		};
	}
}