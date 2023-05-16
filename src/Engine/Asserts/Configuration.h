/*
	This file provides configurable settings
	that can be used to control assert behavior
*/

#ifndef GE3D_ASSERTS_CONFIGURATION_H
#define GE3D_ASSERTS_CONFIGURATION_H

// By default asserts are only enabled for debug builds,
// but you can #define it differently as necessary
#ifdef _DEBUG
	#define GE3D_ASSERTS_AREENABLED
#endif

#ifdef GE3D_ASSERTS_AREENABLED
	// For platforms and debuggers that support it
	// it can be useful to output the assert message to the debugger
	// so that there's a record of the message
	#define GE3D_ASSERTS_SHOULDPRINTTODEBUGGER

#endif

#endif	// GE3D_ASSERTS_CONFIGURATION_H
