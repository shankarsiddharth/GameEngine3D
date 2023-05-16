// Includes
//=========

#include "../cShader.h"

#include <cstdlib>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <new>

// Implementation
//===============

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::Graphics::cShader::Initialize( const std::string& i_path, const Platform::sDataFromFile& i_loadedShader )
{
	auto result = Results::Success;

	GLuint shaderId = 0;
	cScopeGuard scopeGuard( [this, &result, &shaderId]
		{
			if ( shaderId != 0 )
			{
				if ( result )
				{
					m_shaderId = shaderId;
				}
				else
				{
					glDeleteShader( shaderId );
					const auto errorCode = glGetError();
					if ( errorCode != GL_NO_ERROR )
					{
						GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						GE3D::Logging::OutputError( "OpenGL failed to delete the shader ID %u: %s",
							shaderId, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					}
					shaderId = 0;
				}
			}
		} );

	GE3D_ASSERT( ( m_type == GE3D::Graphics::eShaderType::Vertex ) || ( m_type == GE3D::Graphics::eShaderType::Fragment ) );
	const auto shaderType = ( m_type == GE3D::Graphics::eShaderType::Vertex ) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

	// Verify that compiling shaders at run-time is supported
	{
		GLboolean isShaderCompilingSupported;
		glGetBooleanv( GL_SHADER_COMPILER, &isShaderCompilingSupported );
		if ( !isShaderCompilingSupported )
		{
			result = Results::Failure;
			GE3D_ASSERT( false );
			GE3D::Logging::OutputError( "Compiling shaders at run-time isn't supported on this implementation (this should never happen)" );
			return result;
		}
	}

	// Generate a shader
	shaderId = glCreateShader( shaderType );
	{
		const auto errorCode = glGetError();
		if ( errorCode != GL_NO_ERROR )
		{
			result = Results::Failure;
			GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			GE3D::Logging::OutputError( "OpenGL failed to get an unused shader ID: %s",
				reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			return result;
		}
		else if ( shaderId == 0 )
		{
			result = Results::Failure;
			GE3D_ASSERT( false );
			GE3D::Logging::OutputError( "OpenGL failed to get an unused shader ID" );
			return result;
		}
	}
	// Set the source code into the shader
	{
		constexpr GLsizei shaderSourceCount = 1;
		const auto length = static_cast<GLint>( i_loadedShader.size );
		glShaderSource( shaderId, shaderSourceCount, reinterpret_cast<GLchar* const*>( &i_loadedShader.data ), &length );
		const auto errorCode = glGetError();
		if ( errorCode != GL_NO_ERROR )
		{
			result = Results::Failure;
			GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			GE3D::Logging::OutputError( "OpenGL failed to set the shader source code from %s: %s",
				i_path.c_str(), reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			return result;
		}
	}
	// Compile the shader source code
	{
		glCompileShader( shaderId );
		const auto errorCode = glGetError();
		if ( errorCode == GL_NO_ERROR )
		{
			// Get compilation info
			// (this won't be used unless compilation fails
			// but it can be useful to look at when debugging)
			std::string compilationInfo;
			{
				GLint infoSize;
				glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &infoSize );
				const auto errorCode = glGetError();
				if ( errorCode == GL_NO_ERROR )
				{
					if ( infoSize > 0 )
					{
						auto* const info = new (std::nothrow) GLchar[infoSize];
						if ( info )
						{
							GE3D::cScopeGuard scopeGuard_info( [info]
								{
									delete [] info;
								} );
							constexpr GLsizei* const dontReturnLength = nullptr;
							glGetShaderInfoLog( shaderId, static_cast<GLsizei>( infoSize ), dontReturnLength, info );
							const auto errorCode = glGetError();
							if ( errorCode == GL_NO_ERROR )
							{
								compilationInfo = info;
							}
							else
							{
								result = Results::Failure;
								GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
								GE3D::Logging::OutputError( "OpenGL failed to get compilation info about the shader source code of %s: %s",
									i_path.c_str(), reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
								return result;
							}
						}
						else
						{
							result = GE3D::Results::OutOfMemory;
							GE3D_ASSERTF( false, "Couldn't allocate memory for compilation info about the shader source code of %", i_path.c_str() );
							GE3D::Logging::OutputError( "Failed to allocate memory for compilation info about the shader source code of %", i_path.c_str() );
							return result;
						}
					}
				}
				else
				{
					result = Results::Failure;
					GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					GE3D::Logging::OutputError( "OpenGL failed to get the length of the shader compilation info of %s: %s",
						i_path.c_str(), reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
			// Check to see if there were compilation errors
			GLint didCompilationSucceed;
			{
				glGetShaderiv( shaderId, GL_COMPILE_STATUS, &didCompilationSucceed );
				const auto errorCode = glGetError();
				if ( errorCode == GL_NO_ERROR )
				{
					if ( didCompilationSucceed == GL_FALSE )
					{
						result = Results::Failure;
						GE3D_ASSERTF( false, compilationInfo.c_str() );
						GE3D::Logging::OutputError( "OpenGL failed to compile the shader %s: %s",
							i_path.c_str(), compilationInfo.c_str() );
						return result;
					}
				}
				else
				{
					result = Results::Failure;
					GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					GE3D::Logging::OutputError( "OpenGL failed to find if compilation of the shader source code from %s succeeded: %s",
						i_path.c_str(), reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
		}
		else
		{
			result = Results::Failure;
			GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			GE3D::Logging::OutputError( "OpenGL failed to compile the shader source code from %s: %s",
				i_path.c_str(), reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			return result;
		}
	}

	return result;
}

GE3D::cResult GE3D::Graphics::cShader::CleanUp()
{
	auto result = Results::Success;

	if ( m_shaderId != 0 )
	{
		glDeleteShader( m_shaderId );
		const auto errorCode = glGetError();
		if ( errorCode != GL_NO_ERROR )
		{
			if ( result )
			{
				result = Results::Failure;
			}
			GE3D_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			GE3D::Logging::OutputError( "OpenGL failed to delete the shader ID %u: %s",
				m_shaderId, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
		}
		m_shaderId = 0;
	}

	return result;
}
