#pragma once

#include "UniInFlowNode.h"
#include "MultiOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class DesicionNode : public UniInFlowNode, public MultiOutFlowNode
		{

		public:

			DesicionNode(); 
			~DesicionNode();
		};
	}
}