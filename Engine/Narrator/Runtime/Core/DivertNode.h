#pragma once

#include "UniInFlowNode.h"
#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class DivertNode : public UniInFlowNode, public UniOutFlowNode
		{

		public:

			DivertNode(); 
			~DivertNode();

		protected:

			
		};
	}
}