/*
	This file provides more advanced assert functionality than the standard library

	Assertions (or "asserts") are a run-time mechanism of error checking
	that are used during development to verify programmer assumptions.
	You assert that some condition should be true,
	and if that assertion is false the code will stop immediately and tell you so,
	allowing you to find the bug and fix it.
	Asserts are not included in the shipping game (for performance reasons),
	and so they are not a replacement for standard error checking.
	Instead, you should use them as "sanity checks"
	to make sure that something that should never happen never does.
*/

#ifndef GE3D_ASSERTS_H
#define GE3D_ASSERTS_H

// Includes
//=========

#include "Configuration.h"

#ifdef GE3D_ASSERTS_AREENABLED

	#include <sstream>

	#ifdef GE3D_PLATFORM_WINDOWS
		#include <intrin.h>
	#endif

#endif

// Helper Function Declarations
//=============================

#ifdef GE3D_ASSERTS_AREENABLED
	namespace GE3D
	{
		namespace Asserts
		{
			bool ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak( const unsigned int i_lineNumber, const char* const i_file,
				bool& io_shouldThisAssertBeIgnoredInTheFuture, const char* const i_message, ... );
			bool ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak_platformSpecific(
				std::ostringstream& io_message, bool& io_shouldThisAssertBeIgnoredInTheFuture );
		}
	}
#endif

// Interface
//==========

#ifdef GE3D_ASSERTS_AREENABLED
	// Breaking could be defined in Asserts.cpp and avoid any platform-specific code here in the header file,
	// but then the debugger would break in Asserts.cpp rather than in the file where the failed assert is
	#if defined( GE3D_PLATFORM_WINDOWS )
		#define GE3D_ASSERTS_BREAK __debugbreak()
	#else
		#error "No implementation exists for breaking in the debugger when an assert fails"
	#endif

	#define GE3D_ASSERT( i_assertion )	\
	{	\
		static auto shouldThisAssertBeIgnored = false;	\
		if ( !shouldThisAssertBeIgnored && !static_cast<bool>( i_assertion )	\
			&& GE3D::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak( __LINE__, __FILE__, shouldThisAssertBeIgnored, "" ) )	\
		{	\
			GE3D_ASSERTS_BREAK;	\
		}	\
	}
	#define GE3D_ASSERTF( i_assertion, i_messageToDisplayWhenAssertionIsFalse, ... )	\
	{	\
		static auto shouldThisAssertBeIgnored = false;	\
		if ( !shouldThisAssertBeIgnored && !static_cast<bool>( i_assertion ) \
			&& GE3D::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak( __LINE__, __FILE__,	\
				shouldThisAssertBeIgnored, i_messageToDisplayWhenAssertionIsFalse, __VA_ARGS__ ) )	\
		{	\
			GE3D_ASSERTS_BREAK;	\
		}	\
	}
#else
	// The macros do nothing when asserts aren't enabled
	#define GE3D_ASSERT( i_assertion )
	#define GE3D_ASSERTF( i_assertion, i_messageToDisplayWhenAssertionIsFalse, ... )
#endif

#endif	// GE3D_ASSERTS_H
