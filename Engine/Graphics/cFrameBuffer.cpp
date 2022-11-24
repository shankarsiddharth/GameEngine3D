// Includes
//=========

#include "cFrameBuffer.h"

#include "ConstantBufferFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>

// Interface
//==========

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cFrameBuffer::Initialize( const void* const i_initialData )
{
	auto result = Results::Success;

	if ( m_type < FrameBufferTypes::Count )
	{
		// Find the size of the type's struct
		{
			switch ( m_type )
			{
				case FrameBufferTypes::Frame: m_size = sizeof( ConstantBufferFormats::sFrame ); break;
//				case FrameBufferTypes::Material: m_size = sizeof( ConstantBufferFormats::sMaterial ); break;
				case FrameBufferTypes::DrawCall: m_size = sizeof( ConstantBufferFormats::sDrawCall ); break;

			// This should never happen
			default:

				result = Results::Failure;
				EAE6320_ASSERTF( false, "Unrecognized frame buffer type %u", m_type );
				Logging::OutputError( "The size couldn't be calculated for a frame buffer of type %u", m_type );
				return result;
			}
			EAE6320_ASSERT( m_size > 0 );
		}
		// Initialize the platform-specific frame buffer
		{
			result = Initialize_platformSpecific( i_initialData );
			EAE6320_ASSERT( result );
		}
	}
	else
	{
		result = Results::Failure;
		EAE6320_ASSERTF( false, "Invalid frame buffer type %u", m_type );
		Logging::OutputError( "A frame buffer is being initialized with the invalid type %u", m_type );
		return result;
	}

	return result;
}

uint16_t eae6320::Graphics::cFrameBuffer::GetWidth() const
{
	return m_width;
}

void eae6320::Graphics::cFrameBuffer::SetWidth(uint16_t i_width)
{
	m_width = i_width;
}

uint16_t eae6320::Graphics::cFrameBuffer::GetHeight() const
{
	return m_height;
}

void eae6320::Graphics::cFrameBuffer::SetHeight(uint16_t i_height)
{
	m_height = i_height;
}

eae6320::Graphics::cFrameBuffer::cFrameBuffer( const FrameBufferTypes i_type )
	:
	m_type( i_type )
{

}

eae6320::Graphics::cFrameBuffer::~cFrameBuffer()
{
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}
