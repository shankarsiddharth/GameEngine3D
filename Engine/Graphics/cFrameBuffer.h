/*
	A frame buffer is a block of data
	that is copied from the CPU to the GPU

	The name "frame" is because the data is
	frame for every vertex and fragment in a single draw call
	(as opposed to data that will be different in different invocations of a shader,
	like per-vertex data in a vertex shader or interpolated data in a fragment shader).

	It is important to differentiate a "frame buffer" object from the actual frame data:
		* A frame buffer is an object owned by the platform-specific graphics API,
			whose sole purpose is to get data from the CPU to the GPU
		* The frame data is calculated and handled just like any other C++ data in the application
			(and is stored in one of the structs defined in ConstantBufferFormats.h)
			but in order for it to be used in a draw call
			it must use a frame buffer object to be available to the shader programs running on the GPU
*/

#ifndef EAE6320_GRAPHICS_CFRAMEBUFFER_H
#define EAE6320_GRAPHICS_CFRAMEBUFFER_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>
#include <Engine/Results/Results.h>

#ifdef EAE6320_PLATFORM_GL
	#include "OpenGL/Includes.h"
#endif

// Forward Declarations
//=====================

#ifdef EAE6320_PLATFORM_D3D
	struct ID3D11Buffer;
#endif

// Constant Buffer Types
//======================

namespace eae6320
{
	namespace Graphics
	{
		// In our class we will define three different types of frame buffers
		// (see ConstantBufferFormats.h for the data layout of each type)
		enum class FrameBufferTypes : uint8_t
		{
			// These values aren't arbitrary enumerations;
			// they must match the IDs assigned to the corresponding frame buffer definitions in shader code

			//	* Frame:
			//		* These values are frame for the entire rendered frame
			//		* The frame buffer must be updated and bound at the start of a frame,
			//			but then it doesn't ever change
			Frame = 0,
			//	* Material:
			//		* These are values that come from an authored material
			//		* If multiple successive draw calls use the same material
			//			then the frame buffer must be updated and bound before the first one
			//			but doesn't need to change until a new material is rendered
			Material = 1,
			//	* Draw Call:
			//		* These are values that are associated with a specific draw call
			//		* The frame buffer must be updated and bound for every draw call that is made
			DrawCall = 2,

			Count,
			Invalid = Count
		};
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cFrameBuffer
		{
			// Interface
			//==========

		public:

			// Render
			//-------
			
			// When a frame buffer is bound all shaders that use a defined frame buffer with matching ID
			// (i.e. when a C++ frame buffer's type enumeration matches the frame buffer ID defined in the shader)
			// will get data from that frame buffer until a different frame buffer is bound.
			// i_shaderTypesToBindTo is a concatenation of Graphics::ShaderTypes,
			// and specifies which shader types need to use this frame buffer.
			void Bind( const uint_fast8_t i_shaderTypesToBindTo ) const;

			void UnBind() const;

			// Copies the specified CPU data to the GPU memory associated with the frame buffer.
			// The specified data must be the appropriate Graphics::ConstantBufferFormats struct corresponding to this frame buffer's type!
			// This function only needs to be called when the frame data that the GPU is using needs to change.
			void Update( const void* const i_data );

			uint16_t GetWidth() const;
			void SetWidth(uint16_t i_width);
			uint16_t GetHeight() const;
			void SetHeight(uint16_t i_height);

#if defined( EAE6320_PLATFORM_D3D )

#elif defined( EAE6320_PLATFORM_GL )
			GLint GetCurrentFrame() const;
#endif

			// Initialize / Clean Up
			//----------------------

			cResult Initialize( const void* const i_initialData = nullptr );
			cResult CleanUp();

			cFrameBuffer( const FrameBufferTypes i_type );
			~cFrameBuffer();

			// Data
			//=====

		private:

			// The size of the frame data associated with this frame buffer.
			// It is calculated internally as sizeof( Graphics::ConstantBufferFormats::[m_type] )
			size_t m_size = 0;

#if defined( EAE6320_PLATFORM_D3D )
			ID3D11Buffer* m_buffer = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
			GLuint m_bufferId = 0;
			GLuint m_textureColorBufferId = 0;
			GLuint m_renderBufferId = 0;
#endif
			uint16_t m_height = 512, m_width = 512;

			// The frame buffer type defines the size of the frame data
			// and is used to bind the frame buffer (the type enumeration is used as an ID)
			const FrameBufferTypes m_type = FrameBufferTypes::Invalid;

			// Implementation
			//---------------

		private:

			// Initialize / Clean Up
			//----------------------

			cResult Initialize_platformSpecific( const void* const i_initialData );

			cFrameBuffer( const cFrameBuffer& ) = delete;
			cFrameBuffer( cFrameBuffer&& ) = delete;
			cFrameBuffer& operator =( const cFrameBuffer& ) = delete;
			cFrameBuffer& operator =( cFrameBuffer&& ) = delete;
		};
	}
}

#endif	// EAE6320_GRAPHICS_CFRAMEBUFFER_H
