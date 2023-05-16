// Includes
//=========

#include "UserSettings.h"

#include <cmath>
#include <csetjmp>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/UserOutput/UserOutput.h>
#include <External/Lua/Includes.h>
#include <limits>
#include <string>

// Static Data
//============

namespace
{
	uint16_t s_resolutionHeight = 0;
	auto s_resolutionHeight_validity = GE3D::Results::Failure;
	uint16_t s_resolutionWidth = 0;
	auto s_resolutionWidth_validity = GE3D::Results::Failure;

	constexpr auto* const s_userSettingsFileName = "settings.ini";

	// Restore point if Lua panics
	jmp_buf s_jumpBuffer;
}

// Helper Declarations
//====================

namespace
{
	GE3D::cResult InitializeIfNecessary();
	GE3D::cResult LoadUserSettingsIntoLuaTable( lua_State& io_luaState );
	GE3D::cResult PopulateUserSettingsFromLuaTable( lua_State& io_luaState );

	// Called if Lua panics
	// (e.g. when an unhandled error is thrown)
	int OnLuaPanic( lua_State* io_luaState ) noexcept;
}

// Interface
//==========

GE3D::cResult GE3D::UserSettings::GetDesiredInitialResolutionWidth( uint16_t& o_width )
{
	const auto result = InitializeIfNecessary();
	if ( result )
	{
		if ( s_resolutionWidth_validity )
		{
			o_width = s_resolutionWidth;
		}
		return s_resolutionWidth_validity;
	}
	else
	{
		return result;
	}
}

GE3D::cResult GE3D::UserSettings::GetDesiredInitialResolutionHeight( uint16_t& o_height )
{
	const auto result = InitializeIfNecessary();
	if ( result )
	{
		if ( s_resolutionHeight_validity )
		{
			o_height = s_resolutionHeight;
		}
		return s_resolutionHeight_validity;
	}
	else
	{
		return result;
	}
}

// Helper Definitions
//===================

namespace
{
	GE3D::cResult InitializeIfNecessary()
	{
		static GE3D::cResult isInitialized;
		if ( isInitialized != GE3D::Results::Undefined )
		{
			return isInitialized;
		}

		auto result = GE3D::Results::Success;

		// Create a new Lua state
		lua_State* luaState = nullptr;
		auto wasUserSettingsEnvironmentCreated = false;

		const GE3D::cScopeGuard scopeGuard( [&result, &luaState, &wasUserSettingsEnvironmentCreated]()
			{
				// Free the Lua environment
				if ( luaState )
				{
					if ( wasUserSettingsEnvironmentCreated )
					{
						lua_pop( luaState, 1 );
					}
					GE3D_ASSERTF( lua_gettop( luaState ) == 0, "Lua stack is inconsistent" );
					lua_close( luaState );
					luaState = nullptr;
				}
				// Store the result
				isInitialized = result;
			} );

		int jumpValue = 0;	// 0 means no jump has happened
		{
			luaState = luaL_newstate();
			if ( luaState )
			{
				// Set a function that will be called if Lua is about to abort
				lua_atpanic( luaState, OnLuaPanic );
			}
			else
			{
				result = GE3D::Results::OutOfMemory;
				GE3D_ASSERTF( false, "Failed to create a new Lua state" );
				GE3D::Logging::OutputError( "Failed to create a new Lua state for the user settings" );
				return result;
			}
		}
		// Set a restore point in case Lua panics
		jumpValue = setjmp( s_jumpBuffer );
		if ( jumpValue == 0 )
		{
			// Create an empty table to be used as the Lua environment for the user settings
			if ( lua_checkstack( luaState, 1 ) )
			{
				lua_newtable( luaState );
				wasUserSettingsEnvironmentCreated = true;
			}
			else
			{
				result = GE3D::Results::OutOfMemory;
				GE3D_ASSERTF( false, "Lua didn't increase its tack for a new table" );
				GE3D::Logging::OutputError( "User settings files can't be processed"
					" because Lua can't increase its stack for a new table" );
				return result;
			}
			// Load the user settings
			if ( result = LoadUserSettingsIntoLuaTable( *luaState ) )
			{
				// Populate the user settings in C from the user settings in the Lua environment
				if ( !( result = PopulateUserSettingsFromLuaTable( *luaState ) ) )
				{
					return result;
				}
			}
			else
			{
				return result;
			}
		}
		else
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF( false, "Unhandled Lua error" );
			GE3D::Logging::OutputError( "User settings files can't be processed"
				" because of an unhandled Lua error" );
		}

		return result;
	}

	GE3D::cResult LoadUserSettingsIntoLuaTable( lua_State& io_luaState )
	{
		// Load the user settings file into the Lua environment
		if ( GE3D::Platform::DoesFileExist( s_userSettingsFileName ) )
		{
			constexpr int requiredStackSlotCount = 0
				// The file as a function
				+ 1
				// The environment upvalue
				+ 1
				;
			if ( lua_checkstack( &io_luaState, requiredStackSlotCount ) )
			{
				// Load the file and compile its contents into a Lua function
				const auto luaResult = luaL_loadfile( &io_luaState, s_userSettingsFileName );
				if ( luaResult == LUA_OK )
				{
					// Set the Lua function's environment
					{
						// Push the empty table to the top of the stack
						lua_pushvalue( &io_luaState, -2 );
						// Set the empty table as the function's global environment
						// (this means that anything that the file syntactically adds to the global environment
						// will actually be added to the table)
						constexpr int globalEnvironmentUpvalueIndex = 1;
						const auto* const upvalueName = lua_setupvalue( &io_luaState, -2, globalEnvironmentUpvalueIndex );
						if ( upvalueName )
						{
							GE3D_ASSERT( strcmp( "_ENV", upvalueName ) == 0 );
						}
						else
						{
							GE3D_ASSERT( false );
							GE3D::Logging::OutputError( "Internal error setting the Lua environment for the user settings file \"%s\"!"
								" This should never happen", s_userSettingsFileName );
							lua_pop( &io_luaState, 2 );
							return GE3D::Results::Failure;
						}
					}
					// Call the Lua function
					// (this will add anything that the file syntactically sets in the global environment
					// into the empty table that was created)
					{
						constexpr int noArguments = 0;
						constexpr int noReturnValues = 0;
						constexpr int noErrorMessageHandler = 0;
						const auto luaResult = lua_pcall( &io_luaState, noArguments, noReturnValues, noErrorMessageHandler );
						if ( luaResult == LUA_OK )
						{
							return GE3D::Results::Success;
						}
						else
						{
							const std::string luaErrorMessage = lua_tostring( &io_luaState, -1 );
							lua_pop( &io_luaState, 1 );

							GE3D_ASSERTF( false, "User settings file error: %s", luaErrorMessage.c_str() );
							if ( luaResult == LUA_ERRRUN )
							{
								GE3D::Logging::OutputError( "Error in the user settings file \"%s\": %s",
									s_userSettingsFileName, luaErrorMessage );
							}
							else
							{
								GE3D::Logging::OutputError( "Error processing the user settings file \"%s\": %s",
									s_userSettingsFileName, luaErrorMessage );
							}

							return GE3D::Results::InvalidFile;
						}
					}
				}
				else
				{
					const std::string luaErrorMessage = lua_tostring( &io_luaState, -1 );
					lua_pop( &io_luaState, 1 );

					if ( luaResult == LUA_ERRFILE )
					{
						GE3D_ASSERTF( false, "Error opening or reading user settings file: %s", luaErrorMessage.c_str() );
						GE3D::Logging::OutputError( "Error opening or reading the user settings file \"%s\" even though it exists: %s",
							s_userSettingsFileName, luaErrorMessage.c_str() );

					}
					else if ( luaResult == LUA_ERRSYNTAX )
					{
						GE3D_ASSERTF( false, "Syntax error in user settings file: %s", luaErrorMessage.c_str() );
						GE3D::Logging::OutputError( "Syntax error in the user settings file \"%s\": %s",
							s_userSettingsFileName, luaErrorMessage.c_str() );
					}
					else
					{
						GE3D_ASSERTF( false, "Error loading user settings file: %s", luaErrorMessage.c_str() );
						GE3D::Logging::OutputError( "Error loading the user settings file \"%s\": %s",
							s_userSettingsFileName, luaErrorMessage.c_str() );
					}

					return GE3D::Results::InvalidFile;
				}
			}
			else
			{
				GE3D_ASSERTF( false, "Not enough stack space to load user settings file" );
				GE3D::Logging::OutputError( "Lua can't allocate enough stack space to load the user settings file \"%s\"",
					s_userSettingsFileName );
				return GE3D::Results::OutOfMemory;
			}
		}
		else
		{
			// If loading the file failed because the file doesn't exist it's ok;
			// default values will be used
			GE3D::Logging::OutputMessage( "The user settings file \"%s\" doesn't exist. Using default settings instead.",
				s_userSettingsFileName );
			return GE3D::Results::FileDoesntExist;
		}
	}

	GE3D::cResult PopulateUserSettingsFromLuaTable( lua_State& io_luaState )
	{
		auto result = GE3D::Results::Success;

		// There should always be enough stack space because the file had to be loaded,
		// but it doesn't hurt to do a sanity check in the context of this function
		if ( !lua_checkstack( &io_luaState, 1 ) )
		{
			GE3D_ASSERTF( false, "Not enough stack space to read a setting from user settings file" );
			GE3D::Logging::OutputError( "Lua can't allocate enough stack space to read each user setting" );
			return GE3D::Results::OutOfMemory;
		}

		// Resolution Width
		{
			constexpr char* const key_width = "resolutionWidth";

			lua_pushstring( &io_luaState, key_width );
			lua_gettable( &io_luaState, -2 );
			GE3D::cScopeGuard scopeGuard_popWidth( [&io_luaState]
				{
					lua_pop( &io_luaState, 1 );
				} );
			if ( lua_isinteger( &io_luaState, -1 ) )
			{
				const auto luaInteger = lua_tointeger( &io_luaState, -1 );
				if ( luaInteger >= 0 )
				{
					constexpr auto maxWidth = std::numeric_limits<decltype( s_resolutionWidth )>::max();
					if ( luaInteger <= maxWidth )
					{
						s_resolutionWidth = static_cast<uint16_t>( luaInteger );
						s_resolutionWidth_validity = GE3D::Results::Success;
						GE3D::Logging::OutputMessage( "User settings defined resolution width of %u", s_resolutionWidth );
					}
					else
					{
						s_resolutionWidth_validity = GE3D::Results::InvalidFile;
						GE3D::Logging::OutputMessage( "The user settings file %s specifies a resolution width (%i)"
							" that is bigger than the maximum (%u)", s_userSettingsFileName, luaInteger, maxWidth );
					}
				}
				else
				{
					s_resolutionWidth_validity = GE3D::Results::InvalidFile;
					GE3D::Logging::OutputMessage( "The user settings file %s specifies a non-positive resolution width (%i)",
						s_userSettingsFileName, luaInteger );
				}
			}
			else
			{
				s_resolutionWidth_validity = GE3D::Results::InvalidFile;
				GE3D::Logging::OutputMessage( "The user settings file %s specifies a %s for %s instead of an integer",
					s_userSettingsFileName, luaL_typename( &io_luaState, -1 ), key_width );
			}
		}
		// Resolution Height
		{
			const char* key_height = "resolutionHeight";

			lua_pushstring( &io_luaState, key_height );
			lua_gettable( &io_luaState, -2 );
			GE3D::cScopeGuard scopeGuard_popHeight( [&io_luaState]
				{
					lua_pop( &io_luaState, 1 );
				} );
			if ( lua_isinteger( &io_luaState, -1 ) )
			{
				const auto luaInteger = lua_tointeger( &io_luaState, -1 );
				if ( luaInteger >= 0 )
				{
					constexpr auto maxHeight = std::numeric_limits<decltype( s_resolutionHeight )>::max();
					if ( luaInteger <= maxHeight )
					{
						s_resolutionHeight = static_cast<uint16_t>( luaInteger );
						s_resolutionHeight_validity = GE3D::Results::Success;
						GE3D::Logging::OutputMessage( "User settings defined resolution height of %u", s_resolutionHeight );
					}
					else
					{
						s_resolutionHeight_validity = GE3D::Results::InvalidFile;
						GE3D::Logging::OutputMessage( "The user settings file %s specifies a resolution height (%i)"
							" that is bigger than the maximum (%u)", s_userSettingsFileName, luaInteger, maxHeight );
					}
				}
				else
				{
					s_resolutionHeight_validity = GE3D::Results::InvalidFile;
					GE3D::Logging::OutputMessage( "The user settings file %s specifies a non-positive resolution height (%i)",
						s_userSettingsFileName, luaInteger );
				}
			}
			else
			{
				s_resolutionHeight_validity = GE3D::Results::InvalidFile;
				GE3D::Logging::OutputMessage( "The user settings file %s specifies a %s for %s instead of an integer",
					s_userSettingsFileName, luaL_typename( &io_luaState, -1 ), key_height );
			}
		}

		return result;
	}

	int OnLuaPanic( lua_State* io_luaState ) noexcept
	{
		// The error object should be at the top of the stack
		{
			if ( lua_isstring( io_luaState, -1 ) )
			{
				GE3D::Logging::OutputError( "Lua is panicking when processing User Settings: %s", lua_tostring( io_luaState, -1 ) );
			}
			else
			{
				GE3D::Logging::OutputError( "Lua is panicking for an unknown reason when processing User Settings" );
			}
		}
		// Jump back to the restore point
		{
			constexpr int errorCode = -1;	// This value isn't checked
			longjmp( s_jumpBuffer, errorCode );
		}

		// This code should never be reached
		GE3D::Logging::OutputError( "The application was not able to recover from Lua's panicking."
			" It is about to be forcefully exited." );
		GE3D::UserOutput::Print( "An unexpected error occurred when processing User Settings (see the log file for details)."
			" The application is about to exit" );
		return 0;
	}
}
