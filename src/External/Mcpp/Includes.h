/*
	This file can be included by a project that wants to use MCPP

	Using this intermediate file to store the version number
	means that no other source files will have to change
	if you ever change to a new version.
*/

#ifndef GE3D_MCPPINCLUDES_H
#define GE3D_MCPPINCLUDES_H

// Windows and mcpp both define a global "OUT".
// This is an example of why C++ namespaces are a good thing.
#if defined( GE3D_PLATFORM_WINDOWS ) && defined( OUT )
	#define GE3D_WINDOWS_OUT
	#undef OUT
#endif

extern "C"
{
	#include "2.7.2/src/mcpp_lib.h"
}

namespace GE3D
{
	namespace mcpp
	{
		namespace OUTDEST
		{
			enum eOUTDEST
			{
				Out = OUT,
				Err = ERR,
				Dbg = DBG,
			};
		}
	}
}

#ifdef GE3D_WINDOWS_OUT
	#define OUT
	#undef GE3D_WINDOWS_OUT
#endif

#endif	// GE3D_MCPPINCLUDES_H
