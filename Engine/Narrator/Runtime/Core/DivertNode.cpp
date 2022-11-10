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

void Narrator::Runtime::DivertNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::MultiInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);
	
	nlohmann::json target = nlohmann::json::object();
	target["name"] = m_TargetNodeName;
	
	nlohmann::json targetNode = nlohmann::json::object();
	targetNode["id"] = m_TargetNode->GetID();
	targetNode["type"] = m_TargetNode->GetType();
	target["node"] = targetNode;

	nodeObject["target"] = target;
}

std::string Narrator::Runtime::DivertNode::ToString()
{
	return GetName() + " : ->" + m_TargetNodeName;
}
