/*
	This file contains concurrency-related constants
*/

#ifndef GE3D_CONCURRENCY_CONSTANTS_H
#define GE3D_CONCURRENCY_CONSTANTS_H

namespace GE3D
{
	namespace Concurrency
	{
		namespace Constants
		{
			constexpr auto DontTimeOut = ~unsigned int( 0u );
		}
	}
}

#endif	// GE3D_CONCURRENCY_CONSTANTS_H
