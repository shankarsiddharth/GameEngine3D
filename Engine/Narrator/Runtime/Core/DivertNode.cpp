#include "DivertNode.h"

Narrator::Runtime::DivertNode::DivertNode()
	:Narrator::Runtime::UniOutFlowNode(TNodeType::kDivert)
{
	m_NextNode = nullptr;
}

Narrator::Runtime::DivertNode::~DivertNode()
{

}

