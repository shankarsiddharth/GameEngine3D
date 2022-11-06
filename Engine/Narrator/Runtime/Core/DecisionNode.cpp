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
	std::uint32_t pathCount = static_cast<uint32_t>(m_OutFlowNodeMap.size());
	return pathCount;
}

std::string Narrator::Runtime::DecisionNode::ToString()
{
	return GetName() + "\t:DN:";
}
