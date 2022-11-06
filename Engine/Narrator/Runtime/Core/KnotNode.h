#pragma once

#include "UniInFlowNode.h"
#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class KnotNode : public UniInFlowNode, public UniOutFlowNode
		{

		public:

			KnotNode(); 
			~KnotNode();
			
			std::string ToString() override;
			
			void SetName(const std::string& i_Name);

		};
	}
}