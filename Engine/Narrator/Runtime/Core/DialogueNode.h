#pragma once

#include "UniInFlowNode.h"
#include "UniOutFlowNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class DialogueNode : public UniInFlowNode, public UniOutFlowNode
		{

		public:

			DialogueNode(const std::string& i_Text);
			~DialogueNode();

			std::string ToString() override;

			std::string GetText();


			void ToJSON(nlohmann::json& nodeObject) override;

		protected:

			std::string m_Text;
		};
	}
}