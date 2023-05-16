/*
	This file lists external prebuilt libraries that this library depends on
*/

#pragma once

// External Libraries
//===================

#if defined( GE3D_PLATFORM_D3D )
	#pragma comment( lib, "D3DCompiler.lib" )
#elif defined( GE3D_PLATFORM_GL )
	#pragma comment( lib, "Glu32.lib" )
	#pragma comment( lib, "Opengl32.lib" )
#endif
