#pragma once

#include "Includes.h"
#include "cVertexFormat.h"
#include "VertexFormats.h"

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Results/Results.h>

#include <vector>
#include <string>


// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh
		{

		public:

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cMesh);

			// Reference Counting
			//-------------------

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();


			// Draw
			//-------

			void Draw();

			// Factory Method
			//-------

			static cResult CreateMesh(eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexArray, size_t i_vertexArraySize, uint16_t* i_indexArray, size_t i_indexArraySize, cMesh*& o_mesh);

			static cResult CreateMesh(const std::string& i_path, cMesh*& o_mesh);

		private:

			cMesh() = default;
			~cMesh();

			// Initialize / Clean Up
			//----------------------

			cResult Initialize(eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexArray, size_t i_vertexArraySize, uint16_t* i_indexArray, size_t i_indexArraySize);
			cResult CleanUp();

			// Geometry Data
			//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )
			eae6320::Graphics::cVertexFormat* m_vertexFormat = nullptr;

			// A vertex buffer holds the data for each vertex
			ID3D11Buffer* m_vertexBuffer = nullptr;

			// A index buffer holds the index data of the vertices
			ID3D11Buffer* m_indexBuffer = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
			// A vertex buffer holds the data for each vertex
			GLuint m_vertexBufferId = 0;
			// A vertex array encapsulates the vertex data as well as the vertex input layout
			GLuint m_vertexArrayId = 0;
			// A index buffer holds the index data of the vertices
			GLuint m_indexBufferId = 0;
#endif
#endif
			size_t m_indexBufferSize = 0;

			// Reference Count Variable
			//--------------
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

		};
	}
}