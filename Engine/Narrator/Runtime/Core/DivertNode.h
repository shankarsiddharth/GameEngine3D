#pragma once

#include <string>

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
						
			std::string GetTargetNodeName() const;
			void SetTargetNodeName(std::string i_TargetNodeName);

			Narrator::Runtime::Node* GetTargetNode() const;
			void SetTargetNode(Narrator::Runtime::Node* i_TargetNode);

		protected:

			std::string m_TargetNodeName;
			Narrator::Runtime::Node* m_TargetNode;
		};
	}
}