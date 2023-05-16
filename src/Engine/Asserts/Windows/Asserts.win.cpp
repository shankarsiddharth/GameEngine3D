// Includes
//=========

#include "../Asserts.h"

#ifdef GE3D_ASSERTS_AREENABLED
	#include <Engine/Windows/Includes.h>
#endif

// Helper Definitions
//===================

#ifdef GE3D_ASSERTS_AREENABLED

bool GE3D::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak_platformSpecific(
	std::ostringstream& io_message, bool& io_shouldThisAssertBeIgnoredInTheFuture )
{
#ifdef GE3D_ASSERTS_SHOULDPRINTTODEBUGGER
	OutputDebugStringA( io_message.str().c_str() );
#endif

	io_message << "\n\n"
		"Do you want to break into the debugger?"
		" Choose \"Yes\" to break, \"No\" to continue, or \"Cancel\" to disable this assertion until the program exits.";
	const auto result = MessageBoxA( GetActiveWindow(), io_message.str().c_str(), "Assertion Failed!", MB_YESNOCANCEL );
	if ( ( result == IDYES )
		// MessageBox() returns 0 on failure; if this happens the code breaks rather than trying to diagnose why
		|| ( result == 0 ) )
	{
		return true;
	}
	else
	{
		if ( result == IDCANCEL )
		{
			io_shouldThisAssertBeIgnoredInTheFuture = true;
		}
		return false;
	}
}

#endif	// GE3D_ASSERTS_AREENABLED
