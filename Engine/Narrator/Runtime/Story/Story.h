#pragma once

#include "../Core/Graph.h"

namespace Narrator {
	namespace Runtime {
		class ChoiceNode;
	}
}

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
			std::string Read();

			std::vector<Narrator::Runtime::ChoiceNode*> GetChoices();
			void SelectChoice(uint32_t i_ChoiceIndex);

			static Narrator::Runtime::Story Parse(const std::string& i_Path);
			
			void Play();

			bool ToJSONFile(const std::string& i_JSONFilePath);

			bool FromJSONFile(const std::string& i_JSONFilePath);

		protected:

			void ToJSON(nlohmann::json& jsonRoot) override;
			void FromJSON(const nlohmann::json& jsonRoot) override;

			//Parser Methods
			void Traverse() override;
			void BreadthFirstSearch();

			void AddNode(Narrator::Runtime::Node* i_NodeToAdd);
			void LinkEndNode();

			Narrator::Runtime::Node* GetCurrentNode() const;

			bool HasDivertNode(const std::string& i_DivertName);
			//Always use HasDivertNode to check if it is present before accessing the Node
			Narrator::Runtime::Node* GetDivertNode(const std::string& i_DivertName);
			void AddToDivertNodeMap(const std::string& i_DivertName, Narrator::Runtime::Node* i_DivertNode);
			std::map<std::string, Narrator::Runtime::Node*> GetDivertNodeMap();
			
			bool HasKnotNode(const std::string& i_KnotName);
			//Always use HasKnotNode to check if it is present before accessing the Node
			Narrator::Runtime::Node* GetKnotNode(const std::string& i_KnotName);
			void AddToKnotNodeMap(const std::string& i_KnotName, Narrator::Runtime::Node* i_KnotNode);
			std::map<std::string, Narrator::Runtime::Node*> GetKnotNodeMap();

			Narrator::Runtime::Node* GetLastDecisionNode() const;
			Narrator::Runtime::Node* CreateDecisionNode();
			void ClearLastDecisionNode();

			//Parser Meta Data
			//TODO: #NarratorToDo #NarratorMetaData see:#NarratorMetaDataSample Add a data structure to keep track of the line numbers for the nodes to diaplsy the error

			//Story State
			Narrator::Runtime::Node* m_StartNode;
			Narrator::Runtime::Node* m_EndNode;
			Narrator::Runtime::Node* m_CurrentNode;

			Narrator::Runtime::Node* m_LastDesicionNode;
			Narrator::Runtime::Node* m_LastChoiceNode;
			std::uint32_t m_CurrentChoiceIndex = 0;

		};
	}
}