#ifndef GE3D_GRAPHICS_CRENDERSTATE_INL
#define GE3D_GRAPHICS_CRENDERSTATE_INL

// Includes
//=========

#include "cRenderState.h"

// Render State Bits
//==================

// Alpha Transparency
constexpr bool GE3D::Graphics::RenderStates::IsAlphaTransparencyEnabled( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & AlphaTransparency ) != 0;
}
constexpr void GE3D::Graphics::RenderStates::EnableAlphaTransparency( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= AlphaTransparency;
}
constexpr void GE3D::Graphics::RenderStates::DisableAlphaTransparency( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~AlphaTransparency;
}

// Depth Buffering
constexpr bool GE3D::Graphics::RenderStates::IsDepthTestingEnabled( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & DepthTesting ) != 0;
}
constexpr void GE3D::Graphics::RenderStates::EnableDepthTesting( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= DepthTesting;
}
constexpr void GE3D::Graphics::RenderStates::DisableDepthTesting( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~DepthTesting;
}
constexpr bool GE3D::Graphics::RenderStates::IsDepthWritingEnabled( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & DepthWriting ) != 0;
}
constexpr void GE3D::Graphics::RenderStates::EnableDepthWriting( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= DepthWriting;
}
constexpr void GE3D::Graphics::RenderStates::DisableDepthWriting( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~DepthWriting;
}

// Draw Both Triangle Sides
constexpr bool GE3D::Graphics::RenderStates::ShouldBothTriangleSidesBeDrawn( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & DrawBothTriangleSides ) != 0;
}
constexpr void GE3D::Graphics::RenderStates::EnableDrawingBothTriangleSides( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= DrawBothTriangleSides;
}
constexpr void GE3D::Graphics::RenderStates::DisableDrawingBothTriangleSides( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~DrawBothTriangleSides;
}

#endif	// GE3D_GRAPHICS_CRENDERSTATE_INL
