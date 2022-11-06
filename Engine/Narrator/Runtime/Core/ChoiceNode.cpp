#include "ChoiceNode.h"

Narrator::Runtime::ChoiceNode::ChoiceNode()
	:Narrator::Runtime::Node(TNodeType::kChoice),
	m_Index(0)
{

}

Narrator::Runtime::ChoiceNode::~ChoiceNode()
{

}

std::uint32_t Narrator::Runtime::ChoiceNode::GetChoiceIndex()
{
	return m_Index;
}
