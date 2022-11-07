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

		enum class TInFlowType : std::uint16_t
		{
			kNone = 0,
			//Single In Flow Link
			kUniInFlow,
			//Multiple In Flow Links
			kMultiInFlow
		};

		enum class TOutFlowType : std::uint16_t
		{
			kNone = 0,
			//Single Out Flow Link
			kUniOutFlow,
			//Multiple Out Flow Links
			kMultiOutFlow
		};
	}
}