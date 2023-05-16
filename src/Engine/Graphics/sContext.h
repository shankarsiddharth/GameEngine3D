/*
	The graphics "context" holds platform-specific graphics state

	Only a single instance of a context can exist,
	and it is globally accessible.

	You can add data or functions to the context
	if there is anything that you think should be accessible globally.
*/

#ifndef GE3D_GRAPHICS_CCONTEXT_H
#define GE3D_GRAPHICS_CCONTEXT_H

// Includes
//=========

#include "Configuration.h"

#include "Graphics.h"

#include <Engine/Results/Results.h>

#if defined( GE3D_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>

	#if defined( GE3D_PLATFORM_GL )
		#include "OpenGL/Includes.h"
	#endif
#endif

// Forward Declarations
//=====================

#if defined( GE3D_PLATFORM_D3D )
	struct ID3D11Device;
	struct ID3D11DeviceContext;
	struct IDXGISwapChain;
#endif

// Class Declaration
//==================

namespace GE3D
{
	namespace Graphics
	{
		struct sContext
		{
			// Data
			//=====

#if defined( GE3D_PLATFORM_WINDOWS )
			HWND windowBeingRenderedTo = NULL;
#endif

#if defined( GE3D_PLATFORM_D3D )
			// The device is an interface representing a graphics device (i.e. a graphics card)
			ID3D11Device* direct3dDevice = nullptr;
			// A device's immediate context can only be used by the main/render thread
			// (it is not thread safe)
			ID3D11DeviceContext* direct3dImmediateContext = nullptr;
			// A swap chain is like an array (a "chain") of textures
			// that are rendered to in sequence,
			// with a single one being currently displayed
			IDXGISwapChain* swapChain = nullptr;
#elif defined( GE3D_PLATFORM_GL )
			// The device context and OpenGL rendering context are required to use OpenGL with Windows
			// (i.e. they are Windows concepts and wouldn't be used on other platforms that use OpenGL)
			HDC deviceContext = NULL;
			HGLRC openGlRenderingContext = NULL;
#endif

			// Interface
			//==========

			// Access
			//-------

			static sContext g_context;

			// Initialize / Clean Up
			//----------------------

			cResult Initialize( const sInitializationParameters& i_initializationParameters );
			cResult CleanUp();

			~sContext();

			// Implementation
			//===============

		private:

			sContext() = default;
		};
	}
}

#endif	// GE3D_GRAPHICS_CCONTEXT_H
