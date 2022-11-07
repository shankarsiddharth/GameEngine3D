#include "DivertNode.h"

Narrator::Runtime::DivertNode::DivertNode()
	:Narrator::Runtime::Node(TNodeType::kDivert),
	Narrator::Runtime::MultiInFlowNode(TNodeType::kDivert),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kDivert),
	m_TargetNode(nullptr)
{
	m_NextNode = nullptr;
}

Narrator::Runtime::DivertNode::~DivertNode()
{

}

std::string Narrator::Runtime::DivertNode::GetTargetNodeName() const
{
	return m_TargetNodeName;
}

void Narrator::Runtime::DivertNode::SetTargetNodeName(std::string i_TargetNodeName)
{
	m_TargetNodeName = i_TargetNodeName;
}

Narrator::Runtime::Node* Narrator::Runtime::DivertNode::GetTargetNode() const
{
	return m_TargetNode;
}

void Narrator::Runtime::DivertNode::SetTargetNode(Narrator::Runtime::Node* i_TargetNode)
{
	m_TargetNode = i_TargetNode;
}

std::string Narrator::Runtime::DivertNode::ToString()
{
	return GetName() + " : ->" + m_TargetNodeName;
}
