#include "UniInFlowNode.h"

Narrator::Runtime::UniInFlowNode::UniInFlowNode(TNodeType i_Type /*= TNodeType::kUniFlow*/,
	TInFlowType i_InFlowType /*= TInFlowType::kUniInFlow*/)
	:Narrator::Runtime::Node(i_Type),
	m_PreviousNode(nullptr)
{
	m_InFlowType = i_InFlowType;
}

Narrator::Runtime::UniInFlowNode::~UniInFlowNode()
{

}

void Narrator::Runtime::UniInFlowNode::SetPreviousNode(Narrator::Runtime::Node* i_NextNode)
{
	m_PreviousNode = i_NextNode;
}

Narrator::Runtime::Node* Narrator::Runtime::UniInFlowNode::GetPreviousNode()
{
	return m_PreviousNode;
}
