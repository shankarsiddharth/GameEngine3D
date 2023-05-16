/*
	This file provides configurable settings
	that can be used to modify the graphics project
*/

#ifndef GE3D_GRAPHICS_CONFIGURATION_H
#define GE3D_GRAPHICS_CONFIGURATION_H

// Usually device debug info is only enabled on debug builds
#ifdef _DEBUG
	#define GE3D_GRAPHICS_ISDEVICEDEBUGINFOENABLED
#endif

// Debug shaders are useful for debugging,
// but shouldn't be included in the shipping game
#ifdef _DEBUG
	#define GE3D_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

#endif	// GE3D_GRAPHICS_CONFIGURATION_H
