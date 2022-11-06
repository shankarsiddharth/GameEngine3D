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

			bool canRead();
			void Read();

			std::vector<Narrator::Runtime::ChoiceNode*> GetChoices();
			void SelectChoice(uint32_t i_ChoiceIndex);

			//Parser Methods
			void AddNode(Narrator::Runtime::Node* i_NodeToAdd);
			void LinkEndNode();

			Narrator::Runtime::Node* CreateDecisionNode();
			void ClearLastDecisionNode();

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