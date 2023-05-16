// Includes
//=========

#include "cExampleGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>

// Inherited Implementation
//=========================

// Run
//----

void GE3D::cExampleGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		GE3D_ASSERT( result );
	}
}

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::cExampleGame::Initialize()
{
	return Results::Success;
}

GE3D::cResult GE3D::cExampleGame::CleanUp()
{
	return Results::Success;
}
