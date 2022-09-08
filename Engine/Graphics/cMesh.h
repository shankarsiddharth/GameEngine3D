#pragma once
#include <Engine/Results/Results.h>
#include "cVertexFormat.h"

#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )
#include <d3d11.h>
#elif defined( EAE6320_PLATFORM_GL )

#endif
#endif


// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh
		{
			
		public:

			cMesh() = default;
			~cMesh();
		
		public:

			// Initialize / Clean Up
			//----------------------

			cResult Initialize();
			void CleanUp();

			// Draw
			//-------

			void Draw();


		public:

#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )
			// Geometry Data
			//--------------

			eae6320::Graphics::cVertexFormat* s_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* s_vertexBuffer = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
			
#endif
#endif

		};
	}
}