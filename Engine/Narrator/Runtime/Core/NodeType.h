#pragma once

#include <cstdint>

namespace Narrator 
{
	namespace Runtime
	{
		enum class TNodeType : std::uint16_t
		{
			kNodeBase = 0,
			//Start Node
			kStart,
			//End Node
			kEnd,
			//Text Content Node (Dialogues)
			kDialogue,
			//Link Node
			kDivert,
			//Section Node
			kKnot,
			//Choice Node
			kChoice,
			//Decision Node
			kDesicion
		};
	}
}