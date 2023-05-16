// Includes
//=========

#include "cRenderState.h"

// Interface
//==========

// Access
//-------

bool GE3D::Graphics::cRenderState::IsAlphaTransparencyEnabled() const
{
	return RenderStates::IsAlphaTransparencyEnabled( m_bits );
}

bool GE3D::Graphics::cRenderState::IsDepthTestingEnabled() const
{
	return RenderStates::IsDepthTestingEnabled( m_bits );
}

bool GE3D::Graphics::cRenderState::IsDepthWritingEnabled() const
{
	return RenderStates::IsDepthWritingEnabled( m_bits );
}

bool GE3D::Graphics::cRenderState::ShouldBothTriangleSidesBeDrawn() const
{
	return RenderStates::ShouldBothTriangleSidesBeDrawn( m_bits );
}

uint8_t GE3D::Graphics::cRenderState::GetRenderStateBits() const
{
	return m_bits;
}
