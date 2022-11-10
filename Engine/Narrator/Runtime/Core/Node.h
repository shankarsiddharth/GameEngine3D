#pragma once

#include "NodeType.h"

#include <string>

#include "../JSON/Includes.h"

namespace Narrator
{
	namespace Runtime
	{
		class Node
		{

		public:

			static size_t GetTotalNodeCount();
			static	size_t GetMaxNodeCount();

			Node(TNodeType i_Type = TNodeType::kNodeBase);
			virtual ~Node();

			virtual std::string ToString();

			TNodeType GetType() const;
			std::string GetName() const;
			std::uint32_t GetID() const;
			TInFlowType GetInFlowType() const;
			TOutFlowType GetOutFlowType() const;

			bool IsVisited() const;
			void MarkAsVisited();

			virtual void ToJSON(nlohmann::json& nodeObject);

		protected:

			static std::uint32_t m_CurrentIndex;

			void SetName(const std::string& i_Name);

			std::uint32_t m_ID;
			std::string m_Name;
			TNodeType m_Type;
			TInFlowType m_InFlowType;
			TOutFlowType m_OutFlowType;

			//Traversal Data
			bool m_IsVisited;

		private:

		};
	}
}