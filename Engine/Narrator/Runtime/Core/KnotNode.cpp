#include "KnotNode.h"

Narrator::Runtime::KnotNode::KnotNode()
	:Narrator::Runtime::Node(TNodeType::kKnot)
{

}

Narrator::Runtime::KnotNode::~KnotNode()
{

}

void Narrator::Runtime::KnotNode::SetName(const std::string& i_Name)
{
	Narrator::Runtime::Node::SetName(i_Name);
}
