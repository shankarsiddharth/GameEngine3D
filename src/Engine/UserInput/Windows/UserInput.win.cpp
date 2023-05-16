// Includes
//=========

#include "../UserInput.h"

#include <Engine/Windows/Includes.h>

// Interface
//==========

bool GE3D::UserInput::IsKeyPressed( const uint_fast8_t i_keyCode )
{
	const auto keyState = GetAsyncKeyState( i_keyCode );
	constexpr short isKeyDownMask = ~1;
	return ( keyState & isKeyDownMask ) != 0;
}
