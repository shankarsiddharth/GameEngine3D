// Includes
//=========

#include "sContext.h"

#include <Engine/Asserts/Asserts.h>

// Static Data
//============

GE3D::Graphics::sContext GE3D::Graphics::sContext::g_context;

// Interface
//==========

// Initialize / Clean Up
//----------------------

GE3D::Graphics::sContext::~sContext()
{
	const auto result = CleanUp();
	GE3D_ASSERT( result );
}
