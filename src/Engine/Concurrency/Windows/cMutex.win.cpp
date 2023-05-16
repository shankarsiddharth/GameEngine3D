// Includes
//=========

#include "../cMutex.h"

// Interface
//==========

void GE3D::Concurrency::cMutex::Lock()
{
	AcquireSRWLockExclusive( &m_srwLock );
}

GE3D::cResult GE3D::Concurrency::cMutex::LockIfPossible()
{
	return ( TryAcquireSRWLockExclusive( &m_srwLock ) != FALSE ) ? Results::Success : Results::Failure;
}

void GE3D::Concurrency::cMutex::Unlock()
{
	ReleaseSRWLockExclusive( &m_srwLock );
}

// Initialize / Clean Up
//----------------------

GE3D::Concurrency::cMutex::cMutex()
	:
	m_srwLock( SRWLOCK_INIT )
{
	// The initialize function only sets the value to SRWLOCK_INIT (and this is documented),
	// so it is faster to just set that at construction time than to call the function
	// InitializeSRWLock( &m_srwLock );
}

GE3D::Concurrency::cMutex::~cMutex()
{

}
