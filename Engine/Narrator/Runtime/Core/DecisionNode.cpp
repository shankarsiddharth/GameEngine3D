#include "DecisionNode.h"

Narrator::Runtime::DecisionNode::DecisionNode()
	:Narrator::Runtime::Node(TNodeType::kDesicion),
	Narrator::Runtime::UniInFlowNode(TNodeType::kDesicion),
	Narrator::Runtime::MultiOutFlowNode(TNodeType::kDesicion)
{

}

Narrator::Runtime::DecisionNode::~DecisionNode()
{

}

std::uint32_t Narrator::Runtime::DecisionNode::GetDecisionPathCount()
{
	std::uint32_t pathCount = static_cast<uint32_t>(m_OutFlowNodeMap.size());
	return pathCount;
}

void Narrator::Runtime::DecisionNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::MultiOutFlowNode::ToJSON(nodeObject);
}

std::string Narrator::Runtime::DecisionNode::ToString()
{
	return GetName() + "\t:DN:";
}
