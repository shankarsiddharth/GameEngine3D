#pragma once

#include <cstdint>

namespace Narrator 
{
	namespace Runtime
	{
		enum class TNodeType : std::uint16_t
		{
			kNone = 0,
			//Start Node
			kStart,
			//End Node
			kEnd,
			//Text Dialogue Content Node
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