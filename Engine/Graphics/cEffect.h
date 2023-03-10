#pragma once

#include "Includes.h"
#include "cShader.h"
#include "cRenderState.h"

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Results/Results.h>

#include <string>

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{

		public:

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect);

			// Reference Counting
			//-------------------

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();
						
			// Bind
			//-------

			void Bind();

			// Factory Method
			//-------

			static cResult CreateEffect(cEffect*& o_effect, const std::string& i_fragmentShaderPath = "", const std::string& i_vertexShaderPath = "");

		private:

			cEffect() = default;
			~cEffect();

			// Initialize / Clean Up
			//----------------------

			cResult Initialize(const std::string& i_fragmentShaderPath = "", const std::string& i_vertexShaderPath = "");
			cResult CleanUp();

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

			eae6320::Graphics::cShader* m_vertexShader = nullptr;
			eae6320::Graphics::cShader* m_fragmentShader = nullptr;

			eae6320::Graphics::cRenderState m_renderState;

			
#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )			
#elif defined( EAE6320_PLATFORM_GL )
			// Program Data
			//-------------

			GLuint m_programId = 0;
#endif
#endif
			// Reference Count Variable
			//--------------
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

		};
	}
}