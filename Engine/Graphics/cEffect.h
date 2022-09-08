#pragma once

#include <Engine/Results/Results.h>
#include "cShader.h"
#include "cRenderState.h"

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{

		public:

			cEffect() = default;
			~cEffect();

		public:

			// Initialize / Clean Up
			//----------------------

			cResult Initialize();
			cResult CleanUp();

			// Bind
			//-------

			void Bind();


		private:
#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )			
#elif defined( EAE6320_PLATFORM_GL )
			// Create & Link Program
			//----------------------

			cResult InitializeProgram();
			cResult CleanUpProgram();
#endif
#endif

			// Shading Data
			//-------------

			eae6320::Graphics::cShader* s_vertexShader = nullptr;
			eae6320::Graphics::cShader* s_fragmentShader = nullptr;

			eae6320::Graphics::cRenderState s_renderState;

			
#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )			
#elif defined( EAE6320_PLATFORM_GL )
			// Program Data
			//-------------

			GLuint s_programId = 0;
#endif
#endif

		};
	}
}