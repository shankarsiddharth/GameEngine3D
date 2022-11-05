#pragma once

#include "Node.h"

#include <string>
#include <map>
#include <cstdint>

namespace Narrator
{
	namespace Runtime
	{
		class MultiOutFlowNode : public virtual Node
		{

		public:
			
			MultiOutFlowNode(TNodeType i_Type = TNodeType::kNodeBase);
			virtual ~MultiOutFlowNode();

			void AddOutFlowNode(Narrator::Runtime::Node* i_NextNode);
			void RemoveOutFlowNode(Narrator::Runtime::Node* i_NextNode);
			void RemoveOutFlowNode(std::uint32_t i_NextNodeID);
			std::map<std::uint32_t, Narrator::Runtime::Node*> GetOutFlowNodeMap() const;


		protected:

			std::map<std::uint32_t, Narrator::Runtime::Node*> m_OutFlowNodeMap;

		private:

		};
	}
}