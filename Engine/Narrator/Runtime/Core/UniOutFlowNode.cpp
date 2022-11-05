#include "UniOutFlowNode.h"

Narrator::Runtime::UniOutFlowNode::UniOutFlowNode(TNodeType i_Type /*= TNodeType::kUniFlow*/)
	:Narrator::Runtime::Node(i_Type),
	m_NextNode(nullptr)
{

}

Narrator::Runtime::UniOutFlowNode::~UniOutFlowNode()
{

}

void Narrator::Runtime::UniOutFlowNode::SetNextNode(Narrator::Runtime::Node* i_NextNode)
{
	m_NextNode = i_NextNode;
}

Narrator::Runtime::Node* Narrator::Runtime::UniOutFlowNode::GetNextNode()
{
	return m_NextNode;
}
