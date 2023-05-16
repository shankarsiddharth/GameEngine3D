/*
	In OpenGL the vertex format data is encapsulated in a vertex array,
	and so this class isn't necessary
	(and shouldn't be used)
*/

// Includes
//=========

#include "../cVertexFormat.h"

// Interface
//==========

// Render
//-------

void GE3D::Graphics::cVertexFormat::Bind()
{

}

// Implementation
//===============

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::Graphics::cVertexFormat::Initialize( const eVertexType i_type, const char* const i_vertexShaderPath )
{
	return Results::Success;
}

GE3D::cResult GE3D::Graphics::cVertexFormat::CleanUp()
{
	return Results::Success;
}
