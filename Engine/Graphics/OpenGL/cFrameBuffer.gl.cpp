// Includes
//=========

#include "../cFrameBuffer.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Math/Functions.h>

// Interface
//==========

// Render
//-------

void eae6320::Graphics::cFrameBuffer::Bind( const uint_fast8_t ) const
{
	EAE6320_ASSERT( m_bufferId != 0 );

	// OpenGL doesn't have a way to only bind the frame buffer to specific shader types,
	// and so the input parameter isn't used
	glBindFramebuffer( GL_FRAMEBUFFER, m_bufferId );
	EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
}

void eae6320::Graphics::cFrameBuffer::UnBind() const
{
	EAE6320_ASSERT(m_bufferId != 0);

	// OpenGL doesn't have a way to only bind the frame buffer to specific shader types,
	// and so the input parameter isn't used
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
}

void eae6320::Graphics::cFrameBuffer::Update( const void* const i_data )
{
	EAE6320_ASSERT( m_bufferId != 0 );

	// Make the frame buffer active
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cFrameBuffer::CleanUp()
{
	auto result = Results::Success;

	if ( m_bufferId != 0 )
	{
		constexpr GLsizei bufferCount = 1;
		glDeleteRenderbuffers(bufferCount, &m_renderBufferId);
		glDeleteFramebuffers( bufferCount, &m_bufferId );
		const auto errorCode = glGetError();
		if ( errorCode != GL_NO_ERROR )
		{
			result = Results::Failure;
			EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			Logging::OutputError( "OpenGL failed to delete the frame buffer: %s",
				reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
		}
		m_bufferId = 0;
	}

	return result;
}

// Implementation
//===============

GLint eae6320::Graphics::cFrameBuffer::GetCurrentFrame() const
{
	return m_textureColorBufferId;
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cFrameBuffer::Initialize_platformSpecific( const void* const i_initialData )
{
	auto result = Results::Success;

	// Create a frame buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenFramebuffers( bufferCount, &m_bufferId );

		const auto errorCode = glGetError();
		if ( errorCode == GL_NO_ERROR )
		{
			glBindFramebuffer( GL_FRAMEBUFFER, m_bufferId );
			const auto errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				result = Results::Failure;
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				eae6320::Logging::OutputError( "OpenGL failed to bind the new frame buffer %u: %s",
					m_bufferId, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				return result;
			}
		}
		else
		{
			result = Results::Failure;
			EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			eae6320::Logging::OutputError( "OpenGL failed to get an unused frame buffer ID: %s",
				reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			return result;
		}
	}
	// Allocate space and copy the frame data into the frame buffer
	{
		
		glGenTextures(1, &m_textureColorBufferId);
		glBindTexture(GL_TEXTURE_2D, m_textureColorBufferId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetWidth(), GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorBufferId, 0);

		glGenRenderbuffers(1, &m_renderBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetWidth(), GetHeight());                   // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferId); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			const auto errorCode = glGetError();
			result = Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL Framebuffer is not complete %u: %s",
				m_bufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	return result;
}
