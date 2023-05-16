/*
	This file's functions are used to log messages to a file
	that gets generated every time the game is run
*/

#ifndef GE3D_LOGGING_H
#define GE3D_LOGGING_H

// Includes
//=========

#include "Configuration.h"

#include <Engine/Results/Results.h>

// Interface
//==========

namespace GE3D
{
	namespace Logging
	{
		// Output
		//-------

		cResult OutputMessage( const char* const i_message, ... );
		// An error is identical to a message except that the buffer is flushed to disk immediately
		// (this prevents messages from being lost if an application crashes)
		cResult OutputError( const char* const i_errorMessage, ... );

		// Initialization / Clean Up
		//--------------------------

		cResult Initialize();
		cResult CleanUp();
	}
}

#endif	// GE3D_LOGGING_H
