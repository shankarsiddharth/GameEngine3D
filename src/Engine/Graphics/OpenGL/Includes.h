/*
	This file can be included by a project that wants to use OpenGL
*/

#ifndef GE3D_ENGINE_GRAPHICS_OPENGL_INCLUDES_H
#define GE3D_ENGINE_GRAPHICS_OPENGL_INCLUDES_H

#if defined( GE3D_PLATFORM_WINDOWS )
	// windows.h _must_ be #included before GL.h
	#include <Engine/Windows/Includes.h>

	// The names of these two headers are standard for any platform,
	// but the location isn't
	#include <gl/GL.h>
	#include <gl/GLU.h>	// The "U" is for "utility functions"
#endif

// Modern OpenGL requires extensions
#include <External/OpenGlExtensions/OpenGlExtensions.h>

#endif	// GE3D_ENGINE_GRAPHICS_OPENGL_INCLUDES_H
