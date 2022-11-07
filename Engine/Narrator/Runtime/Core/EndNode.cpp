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
