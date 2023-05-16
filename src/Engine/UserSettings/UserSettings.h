/*
	This file manages user-configurable settings
	(ones that the user can set in an INI file)

	The way that this is implemented would only work with something like a PC
	where the user can access and change a text file,
	but you can imagine that similar concepts could be used
	for settings that can be changed with a GUI inside of the game.
*/

#ifndef GE3D_USERSETTINGS_H
#define GE3D_USERSETTINGS_H

// Includes
//=========

#include <cstdint>
#include <Engine/Results/Results.h>

// Interface
//==========

namespace GE3D
{
	namespace UserSettings
	{
		cResult GetDesiredInitialResolutionWidth( uint16_t& o_width );
		cResult GetDesiredInitialResolutionHeight( uint16_t& o_height );
	}
}

#endif	// GE3D_USERSETTINGS_H