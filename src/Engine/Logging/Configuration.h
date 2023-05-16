/*
	This file provides configurable settings
	that can be used to control logging behavior
*/

#ifndef GE3D_LOGGING_CONFIGURATION_H
#define GE3D_LOGGING_CONFIGURATION_H

// Unless you change the path here the log will be generated
// in the same directory as the game's executable
// (which can be nice because it is easy for a user to find)
#define GE3D_LOGGING_PATH "GE3D.log"

// Flushing the logging buffer to disk is expensive,
// but it can be done after every message is output during development
// if an application is crashing so that no messages are lost
#ifdef _DEBUG
	#define GE3D_LOGGING_FLUSHBUFFERAFTEREVERYMESSAGE
#endif

#endif	// GE3D_LOGGING_CONFIGURATION_H
