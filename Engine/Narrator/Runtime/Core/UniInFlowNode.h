#pragma once

#include "Node.h"

#include <string>

namespace Narrator
{
	namespace Runtime
	{
		class UniInFlowNode : public virtual Node
		{

		public:
			
			UniInFlowNode(TNodeType i_Type = TNodeType::kNodeBase, TInFlowType i_InFlowType = TInFlowType::kUniInFlow);
			virtual ~UniInFlowNode();

			//Parser Methods
			void SetPreviousNode(Narrator::Runtime::Node* i_NextNode);
			Narrator::Runtime::Node* GetPreviousNode();
			
		protected:

			Narrator::Runtime::Node* m_PreviousNode;
		
		private:

		};
	}
}