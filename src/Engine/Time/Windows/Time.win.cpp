// Includes
//=========

#include "../Time.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Windows/Includes.h>
#include <Engine/Windows/Functions.h>

// Static Data
//============

namespace
{
	uint64_t s_tickCountPerSecond = 0;
}

// Interface
//==========

// Time
//-----

uint64_t GE3D::Time::GetCurrentSystemTimeTickCount()
{
	LARGE_INTEGER totalTickCountSinceSystemBoot;
	const auto result = QueryPerformanceCounter( &totalTickCountSinceSystemBoot );
	// Microsoft claims that querying the counter will never fail on Windows XP or later:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx
	GE3D_ASSERTF( result != FALSE, "QueryPerformanceCounter() failed" );
	return static_cast<uint64_t>( totalTickCountSinceSystemBoot.QuadPart );
}

double GE3D::Time::ConvertTicksToSeconds( const uint64_t i_tickCount )
{
	GE3D_ASSERT( s_tickCountPerSecond > 0 );
	return static_cast<double>( i_tickCount ) / static_cast<double>( s_tickCountPerSecond );
}

uint64_t GE3D::Time::ConvertSecondsToTicks( const double i_secondCount )
{
	GE3D_ASSERT( s_tickCountPerSecond > 0 );
	return static_cast<uint64_t>( ( i_secondCount * static_cast<double>( s_tickCountPerSecond ) ) + 0.5 );
}

double GE3D::Time::ConvertRatePerSecondToRatePerTick( const double i_rate_perSecond )
{
	GE3D_ASSERT( s_tickCountPerSecond > 0 );
	return i_rate_perSecond / static_cast<double>( s_tickCountPerSecond );
}

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::Time::Initialize()
{
	auto result = Results::Success;

	// Get the frequency of the high-resolution performance counter
	{
		// Microsoft claims that querying the frequency will never fail on Windows XP or later:
		// https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408(v=vs.85).aspx
		LARGE_INTEGER tickCountPerSecond;
		if ( QueryPerformanceFrequency( &tickCountPerSecond ) != FALSE )
		{
			if ( tickCountPerSecond.QuadPart != 0 )
			{
				s_tickCountPerSecond = static_cast<uint64_t>( tickCountPerSecond.QuadPart );
			}
			else
			{
				result = Results::Failure;
				GE3D_ASSERT( false );
				Logging::OutputMessage( "This hardware doesn't support high resolution performance counters!" );
				return result;
			}
		}
		else
		{
			result = Results::Failure;
			const auto errorMessage = Windows::GetLastSystemError();
			GE3D_ASSERTF( false, errorMessage.c_str() );
			Logging::OutputMessage( "Windows failed to query performance frequency: %s", errorMessage.c_str() );
			return result;
		}
	}

	Logging::OutputMessage( "Initialized time" );

	return result;
}

GE3D::cResult GE3D::Time::CleanUp()
{
	return Results::Success;
}
