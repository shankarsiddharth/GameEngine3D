// Includes
//=========

#include "../cMutex_recursive.h"

// Interface
//==========

void GE3D::Concurrency::cMutex_recursive::Lock()
{
	EnterCriticalSection( &m_criticalSection );
}

GE3D::cResult GE3D::Concurrency::cMutex_recursive::LockIfPossible()
{
	return ( TryEnterCriticalSection( &m_criticalSection ) != FALSE ) ? Results::Success : Results::Failure;
}

void GE3D::Concurrency::cMutex_recursive::Unlock()
{
	LeaveCriticalSection( &m_criticalSection );
}

// Initialize / Clean Up
//----------------------

GE3D::Concurrency::cMutex_recursive::cMutex_recursive()
{
	InitializeCriticalSection( &m_criticalSection );
}

GE3D::Concurrency::cMutex_recursive::~cMutex_recursive()
{
	DeleteCriticalSection( &m_criticalSection );
}
