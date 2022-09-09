#pragma once

#include <Engine/Results/Results.h>
#include "Includes.h"
#include "cVertexFormat.h"


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
			cResult CleanUp();

			// Draw
			//-------

			void Draw();


		private:

#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )
			// Geometry Data
			//--------------

			eae6320::Graphics::cVertexFormat* m_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
			// Geometry Data
			//--------------

			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
#endif
#endif

		};
	}
}