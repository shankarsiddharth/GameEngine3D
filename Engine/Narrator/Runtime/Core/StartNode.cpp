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
