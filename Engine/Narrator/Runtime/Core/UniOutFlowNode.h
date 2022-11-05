#pragma once

#include "Node.h"

#include <string>

namespace Narrator
{
	namespace Runtime
	{
		class UniOutFlowNode : public virtual Node
		{

		public:
			
			UniOutFlowNode(TNodeType i_Type = TNodeType::kNodeBase);
			virtual ~UniOutFlowNode();

			void SetNextNode(Narrator::Runtime::Node* i_NextNode);
			Narrator::Runtime::Node* GetNextNode();
			
		protected:

			Narrator::Runtime::Node* m_NextNode;
		
		private:

		};
	}
}