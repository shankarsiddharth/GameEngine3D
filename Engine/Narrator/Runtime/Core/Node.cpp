#include "Node.h"
#include <iostream>

std::uint32_t Narrator::Runtime::Node::m_CurrentIndex = 0;

size_t Narrator::Runtime::Node::GetTotalNodeCount()
{
	return m_CurrentIndex;
}

size_t Narrator::Runtime::Node::GetMaxNodeCount()
{
	return std::numeric_limits<uint32_t>::max();
}

Narrator::Runtime::Node::Node(TNodeType i_Type /*= TNodeType::kNone*/)
	: m_ID(m_CurrentIndex++),
	m_Type(i_Type),
	m_InFlowType(TInFlowType::kNone),
	m_OutFlowType(TOutFlowType::kNone)
{
	m_Name = std::to_string(m_ID);
}

Narrator::Runtime::Node::~Node()
{

}

Narrator::Runtime::TNodeType Narrator::Runtime::Node::GetType() const
{
	return m_Type;
}

std::string Narrator::Runtime::Node::GetName() const
{
	if (m_Name.empty())
	{
		return std::to_string(m_ID);
	}
	return m_Name;
}

std::uint32_t Narrator::Runtime::Node::GetID() const
{
	return m_ID;
}

Narrator::Runtime::TInFlowType Narrator::Runtime::Node::GetInFlowType() const
{
	return m_InFlowType;
}

Narrator::Runtime::TOutFlowType Narrator::Runtime::Node::GetOutFlowType() const
{
	return m_OutFlowType;
}

void Narrator::Runtime::Node::SetName(const std::string& i_Name)
{
	m_Name = i_Name;
}