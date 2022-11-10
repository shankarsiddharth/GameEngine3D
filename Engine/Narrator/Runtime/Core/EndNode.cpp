#include "EndNode.h"

Narrator::Runtime::EndNode::EndNode()
	:Narrator::Runtime::Node(TNodeType::kEnd),
	Narrator::Runtime::MultiInFlowNode(TNodeType::kEnd)
{
	m_Name = "End";
}

Narrator::Runtime::EndNode::~EndNode()
{

}

void Narrator::Runtime::EndNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::MultiInFlowNode::ToJSON(nodeObject);
}
