#include "DialogueNode.h"

Narrator::Runtime::DialogueNode::DialogueNode(const std::string& i_Text)
	:Narrator::Runtime::Node(TNodeType::kDialogue),
	Narrator::Runtime::UniInFlowNode(TNodeType::kDialogue),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kDialogue),
	m_Text(i_Text)
{

}

Narrator::Runtime::DialogueNode::~DialogueNode()
{

}

std::string Narrator::Runtime::DialogueNode::GetText()
{
	return m_Text;
}

std::string Narrator::Runtime::DialogueNode::ToString()
{
	return GetName() + " : " + m_Text;
}
