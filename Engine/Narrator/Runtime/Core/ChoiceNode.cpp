#include "ChoiceNode.h"

Narrator::Runtime::ChoiceNode::ChoiceNode(const std::uint32_t i_Index, const std::string& i_Text)
	:Narrator::Runtime::Node(TNodeType::kChoice),
	Narrator::Runtime::UniInFlowNode(TNodeType::kChoice),
	Narrator::Runtime::UniOutFlowNode(TNodeType::kChoice),
	m_Index(i_Index),
	m_Text(i_Text)
{

}

Narrator::Runtime::ChoiceNode::~ChoiceNode()
{

}

std::uint32_t Narrator::Runtime::ChoiceNode::GetChoiceIndex()
{
	return m_Index;
}

void Narrator::Runtime::ChoiceNode::ToJSON(nlohmann::json& nodeObject)
{
	Narrator::Runtime::Node::ToJSON(nodeObject);
	Narrator::Runtime::UniInFlowNode::ToJSON(nodeObject);
	Narrator::Runtime::UniOutFlowNode::ToJSON(nodeObject);
	
	nlohmann::json choice = nlohmann::json::object();
	choice["id"] = m_Index;
	choice["text"] = m_Text;
	nodeObject["choice"] = choice;
}

std::string Narrator::Runtime::ChoiceNode::ToString()
{
	return GetName() + " : " + m_Text;
}
