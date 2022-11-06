#include "DecisionNode.h"

Narrator::Runtime::DecisionNode::DecisionNode()
	:Narrator::Runtime::MultiOutFlowNode(TNodeType::kDesicion)
{

}

Narrator::Runtime::DecisionNode::~DecisionNode()
{

}

std::uint32_t Narrator::Runtime::DecisionNode::GetDecisionPathCount()
{
	return m_OutFlowNodeMap.size();
}
