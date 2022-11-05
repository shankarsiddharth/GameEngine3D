#include "MultiInFlowNode.h"

Narrator::Runtime::MultiInFlowNode::MultiInFlowNode(TNodeType i_Type /*= TNodeType::kMultiFlow*/)
	: Narrator::Runtime::Node(i_Type)
{

}

Narrator::Runtime::MultiInFlowNode::~MultiInFlowNode()
{

}

void Narrator::Runtime::MultiInFlowNode::AddInFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_InFlowNodeMap.find(nodeID) == m_InFlowNodeMap.end())
		{
			//Node with nodeID not found, add the new Node to the Map
			m_InFlowNodeMap.insert(std::pair<std::uint32_t, Narrator::Runtime::Node*>(nodeID, i_NextNode));
		}
		else
		{
			m_InFlowNodeMap[nodeID] = i_NextNode;
		}
	}
}

void Narrator::Runtime::MultiInFlowNode::RemoveInFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_InFlowNodeMap.find(nodeID) == m_InFlowNodeMap.end())
		{
			//Node with nodeID not found
			return;
		}
		else
		{
			m_InFlowNodeMap.erase(nodeID);
		}
	}
}

void Narrator::Runtime::MultiInFlowNode::RemoveInFlowNode(std::uint32_t i_NextNodeID)
{
	if (m_InFlowNodeMap.find(i_NextNodeID) == m_InFlowNodeMap.end())
	{
		//Node with nodeID not found
		return;
	}
	else
	{
		m_InFlowNodeMap.erase(i_NextNodeID);
	}
}

std::map<std::uint32_t, Narrator::Runtime::Node*> Narrator::Runtime::MultiInFlowNode::GetInFlowNodeMap() const
{
	return m_InFlowNodeMap;
}


