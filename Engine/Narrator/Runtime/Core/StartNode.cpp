#include "StartNode.h"

Narrator::Runtime::StartNode::StartNode()
	:Narrator::Runtime::Node(TNodeType::kStart),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kStart)
{
	m_Name = "Start";
}

Narrator::Runtime::StartNode::~StartNode()
{

}

void Narrator::Runtime::StartNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);
}
