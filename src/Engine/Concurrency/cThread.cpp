// Includes
//=========

#include "cThread.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Initialize / Clean Up
//----------------------

GE3D::Concurrency::cThread::~cThread()
{
	const auto result = CleanUp();
	GE3D_ASSERT( result );
}
