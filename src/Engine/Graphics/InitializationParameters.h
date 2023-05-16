#pragma once

#include <cstdint>

#if defined( GE3D_PLATFORM_WINDOWS )
#include <Engine/Windows/Includes.h>
#endif

namespace GE3D
{
	namespace Graphics
	{
		struct sInitializationParameters
		{
#if defined( GE3D_PLATFORM_WINDOWS )
			HWND mainWindow = NULL;
#if defined( GE3D_PLATFORM_D3D )
			uint16_t resolutionWidth = 0, resolutionHeight = 0;
#elif defined( GE3D_PLATFORM_GL )
			HINSTANCE thisInstanceOfTheApplication = NULL;
#endif
#endif
		};
	}
}