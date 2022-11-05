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
	:m_Type(i_Type),
	m_ID(m_CurrentIndex++)
{
	
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

