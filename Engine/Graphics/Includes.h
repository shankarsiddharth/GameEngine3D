#pragma once

#if defined( EAE6320_PLATFORM_D3D )
	#if defined( EAE6320_PLATFORM_WINDOWS )
		// windows.h should be #included before the Direct3D header files
		#include <Engine/Windows/Includes.h>

		#include <d3d11.h>
		#include <dxgi.h>
	#endif
#elif defined( EAE6320_PLATFORM_GL )
	#if defined( EAE6320_PLATFORM_WINDOWS )
		// windows.h _must_ be #included before GL.h
		#include <Engine/Windows/Includes.h>

		// The names of these two headers are standard for any platform,
		// but the location isn't
		#include <gl/GL.h>
		#include <gl/GLU.h>	// The "U" is for "utility functions"
	#endif

	// Modern OpenGL requires extensions
	#include <External/OpenGlExtensions/OpenGlExtensions.h>
#endif