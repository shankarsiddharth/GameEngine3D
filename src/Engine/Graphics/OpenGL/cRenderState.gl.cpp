// Includes
//=========

#include "../cRenderState.h"

#include "Includes.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Render
//-------

void GE3D::Graphics::cRenderState::Bind() const
{
	const auto renderStateBits = m_bits;

	// Alpha Transparency
	if ( RenderStates::IsAlphaTransparencyEnabled( renderStateBits ) )
	{
		glEnable( GL_BLEND );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
		// result = ( source * source.a ) + ( destination * ( 1 - source.a ) )
		glBlendEquation( GL_FUNC_ADD );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
	else
	{
		glDisable( GL_BLEND );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
	// Depth Buffering
	if ( RenderStates::IsDepthTestingEnabled( renderStateBits ) )
	{
		// The new fragment becomes a pixel if its depth is less than what has previously been written
		glEnable( GL_DEPTH_TEST );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
		glDepthFunc( GL_LESS );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
	else
	{
		// Don't test the depth buffer
		glDisable( GL_DEPTH_TEST );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
	if ( RenderStates::IsDepthWritingEnabled( renderStateBits ) )
	{
		// Write to the depth buffer
		glDepthMask( GL_TRUE );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
	else
	{
		// Don't write to the depth buffer
		glDepthMask( GL_FALSE );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
	// Draw Both Triangle Sides
	if ( RenderStates::ShouldBothTriangleSidesBeDrawn( renderStateBits ) )
	{
		// Don't cull any triangles
		glDisable( GL_CULL_FACE );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
	else
	{
		// Cull triangles that are facing backwards
		glEnable( GL_CULL_FACE );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
		// Triangles use right-handed winding order
		// (opposite from Direct3D)
		glFrontFace( GL_CCW );
		GE3D_ASSERT( glGetError() == GL_NO_ERROR );
	}
}

// Implementation
//===============

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::Graphics::cRenderState::Initialize( const uint8_t i_renderStateBits )
{
	m_bits = i_renderStateBits;

	// OpenGL uses the bits directly at binding time
	return Results::Success;
}

GE3D::Graphics::cRenderState::~cRenderState()
{

}
