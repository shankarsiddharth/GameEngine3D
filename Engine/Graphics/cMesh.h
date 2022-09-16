#pragma once

#include "Includes.h"
#include "cVertexFormat.h"
#include "VertexFormats.h"

#include <Engine/Results/Results.h>

#include <vector>


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

			cResult Initialize(eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexArray, size_t i_vertexArraySize,uint16_t* i_indexArray, size_t i_indexArraySize);
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

			// A index buffer holds the index data of the vertices
			ID3D11Buffer* m_indexBuffer = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
			// Geometry Data
			//--------------

			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			// A index buffer holds the index data of the vertices
			GLuint m_indexBufferId = 0;
#endif
#endif
			size_t m_indexBufferSize;

		};
	}
}