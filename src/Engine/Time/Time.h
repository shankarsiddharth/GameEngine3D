/*
	This file manages time-related functionality
*/

#ifndef GE3D_TIME_H
#define GE3D_TIME_H

// Includes
//=========

#include <cstdint>
#include <Engine/Results/Results.h>

// Interface
//==========

namespace GE3D
{
	namespace Time
	{
		// Time
		//-----

		uint64_t GetCurrentSystemTimeTickCount();

		double ConvertTicksToSeconds( const uint64_t i_tickCount );
		uint64_t ConvertSecondsToTicks( const double i_secondCount );
		double ConvertRatePerSecondToRatePerTick( const double i_rate_perSecond );

		// Initialize / Clean Up
		//----------------------

		cResult Initialize();
		cResult CleanUp();
	}
}

#endif	// GE3D_TIME_H
