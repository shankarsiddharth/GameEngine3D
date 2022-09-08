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
			// Shading Data
			//-------------

			eae6320::Graphics::cShader* s_vertexShader = nullptr;
			eae6320::Graphics::cShader* s_fragmentShader = nullptr;

			eae6320::Graphics::cRenderState s_renderState;

		};
	}
}