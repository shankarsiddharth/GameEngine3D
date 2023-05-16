// Includes
//=========

#include "cEvent.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Initialize / Clean Up
//----------------------

GE3D::Concurrency::cEvent::~cEvent()
{
	const auto result = CleanUp();
	GE3D_ASSERT( result );
}
