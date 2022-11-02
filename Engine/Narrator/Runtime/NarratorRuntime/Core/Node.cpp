#include "Node.h"

uint32_t Narrator::Runtime::Node::m_CurrentNodeIndex = 0;

size_t Narrator::Runtime::Node::GetTotalNodeCount()
{
	return m_CurrentNodeIndex;
}

size_t Narrator::Runtime::Node::GetMaxNodeCount()
{
	return std::numeric_limits<uint32_t>::max();
}

Narrator::Runtime::Node::Node(TNodeType i_NodeType)
	:m_NodeType(i_NodeType),
	m_NodeID(m_CurrentNodeIndex++)
{

}

Narrator::Runtime::Node::~Node()
{

}

Narrator::Runtime::TNodeType Narrator::Runtime::Node::GetType()
{
	return m_NodeType;
}

std::string Narrator::Runtime::Node::GetName()
{
	if (m_NodeName.empty())
	{
		return std::to_string(m_NodeID);
	}
	return m_NodeName;
}

