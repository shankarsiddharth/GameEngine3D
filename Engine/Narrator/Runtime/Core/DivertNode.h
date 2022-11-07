#pragma once

#include <string>

#include "MultiInFlowNode.h"
#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class DivertNode : public MultiInFlowNode, public UniOutFlowNode
		{

		public:

			DivertNode();
			~DivertNode();

			std::string ToString() override;

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