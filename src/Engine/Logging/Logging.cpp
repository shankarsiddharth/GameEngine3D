// Includes
//=========

#include "Logging.h"

#include <cstdarg>
#include <cstdio>
#include <Engine/Asserts/Asserts.h>
#include <fstream>
#include <sstream>
#include <string>

// Helper Class Declaration
//=========================

namespace
{
	class cLogging
	{
		// Interface
		//----------

	public:

		// Logging
		GE3D::cResult OutputMessage( const std::string& i_message );
		void FlushLog();
		// Initialize / Clean Up
		GE3D::cResult InitializeIfNecesary();
		static void CleanUp();
		cLogging() = default;
		~cLogging();

		// Data
		//-----

	private:

		std::ofstream m_outputStream;

		// Implementation
		//---------------

	private:

		// Initialize / Clean Up
		cLogging( const cLogging& ) = delete;
		cLogging( cLogging&& ) = delete;
		cLogging& operator =( const cLogging& ) = delete;
		cLogging& operator =( cLogging&& ) = delete;
	};
}

// Static Data
//============

namespace
{
	cLogging s_logger;
	auto s_hasTheLoggerBeenDestroyed = false;
	auto s_hasTheLogFileAlreadyBeenWrittenTo = false;
	auto s_cleanUpResult = GE3D::Results::Failure;
}

// Helper Declarations
//====================

namespace
{
	GE3D::cResult OutputMessage( const char* const i_message, va_list io_insertions );
}

// Interface
//==========

// Output
//-------

GE3D::cResult GE3D::Logging::OutputMessage( const char* const i_message, ... )
{
	GE3D::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_message );
		result = ::OutputMessage( i_message, insertions );
		va_end( insertions );
	}
#ifdef GE3D_LOGGING_FLUSHBUFFERAFTEREVERYMESSAGE
	s_logger.FlushLog();
#endif
	return result;
}

GE3D::cResult GE3D::Logging::OutputError( const char* const i_errorMessage, ... )
{
	GE3D::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_errorMessage );
		result = ::OutputMessage( i_errorMessage, insertions );
		va_end( insertions );
	}
	s_logger.FlushLog();
	return result;
}

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::Logging::Initialize()
{
	return s_logger.InitializeIfNecesary();
}

GE3D::cResult GE3D::Logging::CleanUp()
{
	cLogging::CleanUp();
	return s_cleanUpResult;
}

// Helper Class Definition
//========================

namespace
{
	// Interface
	//----------

	// Logging

	GE3D::cResult cLogging::OutputMessage( const std::string& i_message )
	{
		if ( InitializeIfNecesary() )
		{
			// Write the message to the file
			m_outputStream << i_message << "\n";

			return GE3D::Results::Success;
		}
		else
		{
			return GE3D::Results::Failure;
		}
	}

	inline void cLogging::FlushLog()
	{
		m_outputStream.flush();
	}

	// Initialize / Clean Up

	GE3D::cResult cLogging::InitializeIfNecesary()
	{
		// If there is an error when the application is exiting
		// the logger may have already been destroyed
		if ( !s_hasTheLoggerBeenDestroyed )
		{
			if ( m_outputStream.is_open() )
			{
				return GE3D::Results::Success;
			}
			else
			{
				if ( !s_hasTheLogFileAlreadyBeenWrittenTo )
				{
					GE3D_ASSERTF( strlen( GE3D_LOGGING_PATH ) > 0, "The path to log to is empty" );
					m_outputStream.open( GE3D_LOGGING_PATH );
					if ( m_outputStream.is_open() )
					{
						s_hasTheLogFileAlreadyBeenWrittenTo = true;
						GE3D::Logging::OutputMessage( "Opened log file \"%s\"", GE3D_LOGGING_PATH );
						FlushLog();
						return GE3D::Results::Success;
					}
					else
					{
						return GE3D::Results::Failure;
					}
				}
				else
				{
					m_outputStream.open( GE3D_LOGGING_PATH, std::ofstream::app );
					const auto result = m_outputStream.is_open() ? GE3D::Results::Success : GE3D::Results::Failure;
					GE3D_ASSERT( result );
					if ( result )
					{
						GE3D::Logging::OutputMessage( "Re-opened log file" );
						FlushLog();
					}
					return result;
				}
			}
		}
		else
		{
			// If the logger has already been destroyed it needs to be re-constructed in the same memory location
			{
				new ( this ) cLogging;
				// Register the CleanUp() function so that it still gets called when the application exits
				// (the destructor won't be called automatically when using placement new)
				{
					const auto result_atExit = atexit( CleanUp );
					if ( result_atExit == 0 )
					{
						s_hasTheLoggerBeenDestroyed = false;
					}
					else
					{
						GE3D_ASSERTF( false, "Calling atexit() to register logging clean up on a revived logger failed with a return value of %i", result_atExit );
						CleanUp();
						return GE3D::Results::Failure;
					}
				}
			}
			// Re-open the file
			m_outputStream.open( GE3D_LOGGING_PATH, std::ofstream::app );
			const auto result = m_outputStream.is_open() ? GE3D::Results::Success : GE3D::Results::Failure;
			GE3D_ASSERT( result );
			if ( result )
			{
				GE3D::Logging::OutputMessage( "Re-opened log file after it had been destroyed" );
				FlushLog();
			}
			return result;
		}
	}

	void cLogging::CleanUp()
	{
		if ( s_logger.m_outputStream.is_open() )
		{
			{
				GE3D::Logging::OutputMessage( "Closing log file" );
				s_logger.FlushLog();
			}
			s_logger.m_outputStream.close();
			s_cleanUpResult = !s_logger.m_outputStream.is_open() ? GE3D::Results::Success : GE3D::Results::Failure;
			GE3D_ASSERTF( s_cleanUpResult, "Log file wasn't closed" );
			if ( !s_cleanUpResult )
			{
				GE3D::Logging::OutputError( "Error: Log file did not close" );
			}
			return;
		}

		s_cleanUpResult = GE3D::Results::Success;
	}

	cLogging::~cLogging()
	{
		s_hasTheLoggerBeenDestroyed = true;
		CleanUp();
	}
}

// Helper Definitions
//===================

namespace
{
	GE3D::cResult OutputMessage( const char* const i_message, va_list io_insertions )
	{
		constexpr size_t bufferSize = 512;
		char buffer[bufferSize];
		const auto result_format = vsnprintf( buffer, bufferSize, i_message, io_insertions );
		if ( result_format >= 0 )
		{
			if ( result_format < bufferSize )
			{
				return s_logger.OutputMessage( buffer );
			}
			else
			{
				GE3D_ASSERTF( false, "The internal logging buffer of size %u was not big enough to hold the formatted message of length %i",
					bufferSize, result_format + 1 );
				std::ostringstream errorMessage;
				errorMessage << "FORMATTING ERROR! (The internal logging buffer of size " << bufferSize
					<< " was not big enough to hold the formatted message of length " << ( result_format + 1 ) << ".)"
					" Cut-off message is:\n\t" << buffer;
				s_logger.OutputMessage( errorMessage.str().c_str() );
				// Return failure regardless of whether the unformatted message was output
				return GE3D::Results::Failure;
			}
		}
		else
		{
			GE3D_ASSERTF( false, "An encoding error occurred while logging the message \"%s\"", i_message );
			std::ostringstream errorMessage;
			errorMessage << "ENCODING ERROR! Unformatted message was:\n\t" << i_message;
			s_logger.OutputMessage( errorMessage.str().c_str() );
			// Return failure regardless of whether the unformatted message was output
			return GE3D::Results::Failure;
		}
	}
}