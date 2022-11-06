#include "MultiOutFlowNode.h"

Narrator::Runtime::MultiOutFlowNode::MultiOutFlowNode(TNodeType i_Type /*= TNodeType::kMultiFlow*/,
	TOutFlowType i_OutFlowType /*= TOutFlowType::kMultiOutFlow*/)
	: Narrator::Runtime::Node(i_Type)
{
	m_OutFlowType = i_OutFlowType;
}

Narrator::Runtime::MultiOutFlowNode::~MultiOutFlowNode()
{

}

void Narrator::Runtime::MultiOutFlowNode::AddOutFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_OutFlowNodeMap.find(nodeID) == m_OutFlowNodeMap.end())
		{
			//Node with nodeID not found, add the new Node to the Map
			m_OutFlowNodeMap.insert(std::pair<std::uint32_t, Narrator::Runtime::Node*>(nodeID, i_NextNode));
		}
		else
		{
			m_OutFlowNodeMap[nodeID] = i_NextNode;
		}
	}
}

void Narrator::Runtime::MultiOutFlowNode::RemoveOutFlowNode(Narrator::Runtime::Node* i_NextNode)
{
	if (i_NextNode)
	{
		std::uint32_t nodeID = i_NextNode->GetID();
		if (m_OutFlowNodeMap.find(nodeID) == m_OutFlowNodeMap.end())
		{
			//Node with nodeID not found
			return;
		}
		else
		{
			m_OutFlowNodeMap.erase(nodeID);
		}
	}
}

void Narrator::Runtime::MultiOutFlowNode::RemoveOutFlowNode(std::uint32_t i_NextNodeID)
{
	if (m_OutFlowNodeMap.find(i_NextNodeID) == m_OutFlowNodeMap.end())
	{
		//Node with nodeID not found
		return;
	}
	else
	{
		m_OutFlowNodeMap.erase(i_NextNodeID);
	}
}

std::map<std::uint32_t, Narrator::Runtime::Node*> Narrator::Runtime::MultiOutFlowNode::GetOutFlowNodeMap() const
{
	return m_OutFlowNodeMap;
}


