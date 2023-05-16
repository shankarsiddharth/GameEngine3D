#pragma once

#include "Includes.h"
#include "cShader.h"
#include "cRenderState.h"

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Results/Results.h>

#include <string>

// Class Declaration
//==================

namespace GE3D
{
	namespace Graphics
	{
		class cEffect
		{

		public:

			GE3D_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect);

			// Reference Counting
			//-------------------

			GE3D_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();
						
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

#if defined( GE3D_PLATFORM_WINDOWS )			
#if defined( GE3D_PLATFORM_D3D )			
#elif defined( GE3D_PLATFORM_GL )
			// Create & Link Program
			//----------------------

			cResult InitializeProgram();
			cResult CleanUpProgram();
#endif
#endif

			// Shading Data
			//-------------

			GE3D::Graphics::cShader* m_vertexShader = nullptr;
			GE3D::Graphics::cShader* m_fragmentShader = nullptr;

			GE3D::Graphics::cRenderState m_renderState;

			
#if defined( GE3D_PLATFORM_WINDOWS )			
#if defined( GE3D_PLATFORM_D3D )			
#elif defined( GE3D_PLATFORM_GL )
			// Program Data
			//-------------

			GLuint m_programId = 0;
#endif
#endif
			// Reference Count Variable
			//--------------
			GE3D_ASSETS_DECLAREREFERENCECOUNT();

		};
	}
}