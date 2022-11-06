#pragma once

#include "../Core/Graph.h"
#include "../Core/ChoiceNode.h"

namespace Narrator
{
	namespace Runtime
	{
		class Story : public Narrator::Runtime::Graph
		{

		public:
			
			Story();
			virtual ~Story();

			//Runtime Methods
			bool canRead();
			void Read();

			std::vector<Narrator::Runtime::ChoiceNode*> GetChoices();
			void SelectChoice(uint32_t i_ChoiceIndex);

			//Parser Methods
			void AddNode(Narrator::Runtime::Node* i_NodeToAdd);
			void LinkEndNode();

			bool HasDivertNode(const std::string& i_DivertName);
			bool HasKnotNode(const std::string& i_KnotName);

			//Always use HasDivertNode to check if it is present before accessing the Node
			Narrator::Runtime::Node* GetDivertNode(const std::string& i_DivertName);

			Narrator::Runtime::Node* CreateDecisionNode();
			void ClearLastDecisionNode();

			void Traverse() override;

		private:
			//Story State
			Narrator::Runtime::Node* m_StartNode;
			Narrator::Runtime::Node* m_EndNode;
			Narrator::Runtime::Node* m_CurrentNode;

			Narrator::Runtime::Node* m_LastDesicionNode;
			std::uint32_t m_CurrentChoiceIndex = 0;

		

		};
	}
}