#include "ChoiceNode.h"

Narrator::Runtime::ChoiceNode::ChoiceNode(const std::uint32_t i_Index, const std::string& i_Text)
	:Narrator::Runtime::Node(TNodeType::kChoice),
	m_Index(i_Index),
	m_Text(i_Text)
{

}

Narrator::Runtime::ChoiceNode::~ChoiceNode()
{

}

std::uint32_t Narrator::Runtime::ChoiceNode::GetChoiceIndex()
{
	return m_Index;
}

std::string Narrator::Runtime::ChoiceNode::ToString()
{
	return GetName() + " : " + m_Text;
}
