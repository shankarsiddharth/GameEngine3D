#pragma once

#include "Includes.h"
#include "cVertexFormat.h"
#include "ConstantBufferFormats.h"
#include "cConstantBuffer.h"

#include <Engine/Results/Results.h>


// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cView
		{

		public:

			cView() = default;
			~cView();

		public:

			void Clear(float i_red = 0.0f, float i_green = 0.0f, float i_blue = 0.0f, float i_alpha = 1.0f);

			void Swap();

			// Initialize / Clean Up
			//----------------------

			cResult Initialize(const unsigned int i_resolutionWidth = 0, const unsigned int i_resolutionHeight = 0);
			cResult CleanUp();
			
		private:

#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )
			// In Direct3D "views" are objects that allow a texture to be used a particular way:
			// A render target view allows a texture to have color rendered to it
			ID3D11RenderTargetView* m_renderTargetView = nullptr;
			// A depth/stencil view allows a texture to have depth rendered to it
			ID3D11DepthStencilView* m_depthStencilView = nullptr;
#elif defined( EAE6320_PLATFORM_GL )

#endif
#endif

		};
	}
}