#pragma once

#include "NodeType.h"
#include "Node.h"

namespace Narrator
{
	namespace Runtime
	{
		class NodeFactory
		{

		public:

			static Narrator::Runtime::Node* Create(Narrator::Runtime::TNodeType i_NodeType);

		};
	}
}