// Includes
//=========

#include "../OpenGlExtensions.h"

#include <sstream>
#include <Engine/Asserts/Asserts.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Windows/Functions.h>
#include <Engine/Windows/OpenGl.h>

// Helper Declarations
//====================

namespace
{
	const void* GetGlFunctionAddress( const char* const i_functionName, std::string* const o_errorMessage );
}

// Interface
//==========

// OpenGL Extension Definitions
//-----------------------------

PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = nullptr;
PFNGLBINDSAMPLERPROC glBindSampler = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLBLENDEQUATIONPROC glBlendEquation = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::OpenGlExtensions::Load( std::string* const o_errorMessage )
{
	auto result = Results::Success;
	{
		// Load a hidden window
		// (The Windows OpenGL implementation requires an OpenGL window to be created
		// before any of the functions can be called,
		// and so we create a hidden window in order to load the extensions
		// and then destroy it afterwards,
		// all before doing anything with the main window)
		HINSTANCE applicationInstance = NULL;
		Windows::OpenGl::sHiddenWindowInfo hiddenWindowInfo;
		cScopeGuard scopeGuard_hiddenWindowInfo( [o_errorMessage, &result, &applicationInstance, &hiddenWindowInfo]
			{
				const auto result_hiddenWindowInfo = Windows::OpenGl::FreeHiddenContextWindow( applicationInstance, hiddenWindowInfo, o_errorMessage );
				if ( !result_hiddenWindowInfo )
				{
					GE3D_ASSERTF( false, o_errorMessage->c_str() );
					if ( result )
					{
						result = result_hiddenWindowInfo;
					}
				}
			} );
		if ( result = Windows::OpenGl::CreateHiddenContextWindow( applicationInstance, hiddenWindowInfo, o_errorMessage ) )
		{
			GE3D_ASSERTF( wglGetCurrentContext(), "OpenGL extensions can't be loaded without a current OpenGL context" );
		}
		else
		{
			GE3D_ASSERTF( false, o_errorMessage->c_str() );
			return result;
		}

		// Load each extension
#define GE3D_OPENGLEXTENSIONS_LOADFUNCTION( i_functionName, i_functionType )											\
			i_functionName = static_cast<i_functionType>( GetGlFunctionAddress( #i_functionName, o_errorMessage ) );	\
			if ( !i_functionName )																						\
			{																											\
				result = Results::Failure;																				\
				return result;																							\
			}

		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glActiveTexture, PFNGLACTIVETEXTUREPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glAttachShader, PFNGLATTACHSHADERPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glBindBuffer, PFNGLBINDBUFFERPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glBindBufferBase, PFNGLBINDBUFFERBASEPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glBindSampler, PFNGLBINDSAMPLERPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glBindVertexArray, PFNGLBINDVERTEXARRAYPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glBlendEquation, PFNGLBLENDEQUATIONPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glBufferData, PFNGLBUFFERDATAPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glBufferSubData, PFNGLBUFFERSUBDATAPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glCompileShader, PFNGLCOMPILESHADERPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glCompressedTexImage2D, PFNGLCOMPRESSEDTEXIMAGE2DPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glCreateProgram, PFNGLCREATEPROGRAMPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glCreateShader, PFNGLCREATESHADERPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glDeleteBuffers, PFNGLDELETEBUFFERSPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glDeleteProgram, PFNGLDELETEPROGRAMPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glDeleteSamplers, PFNGLDELETESAMPLERSPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glDeleteShader, PFNGLDELETESHADERPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYARBPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGenBuffers, PFNGLGENBUFFERSPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGenSamplers, PFNGLGENSAMPLERSPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGetProgramiv, PFNGLGETPROGRAMIVPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGetShaderiv, PFNGLGETSHADERIVPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glInvalidateBufferData, PFNGLINVALIDATEBUFFERDATAPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glLinkProgram, PFNGLLINKPROGRAMPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glSamplerParameteri, PFNGLSAMPLERPARAMETERIPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glShaderSource, PFNGLSHADERSOURCEPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUniform1fv, PFNGLUNIFORM1FVPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUniform1i, PFNGLUNIFORM1IPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUniform2fv, PFNGLUNIFORM2FVPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUniform3fv, PFNGLUNIFORM3FVPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUniform4fv, PFNGLUNIFORM4FVPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUniformBlockBinding, PFNGLUNIFORMBLOCKBINDINGPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glUseProgram, PFNGLUSEPROGRAMPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC );
		GE3D_OPENGLEXTENSIONS_LOADFUNCTION( wglCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC );

#undef GE3D_OPENGLEXTENSIONS_LOADFUNCTION
	}
	return result;
}

// Helper Declarations
//====================

namespace
{
	const void* GetGlFunctionAddress( const char* const i_functionName, std::string* const o_errorMessage )
	{
		const auto* const address = static_cast<void*>( wglGetProcAddress( i_functionName ) );
		// The documentation says that NULL will be returned if the function isn't found,
		// but according to https://www.opengl.org/wiki/Load_OpenGL_Functions
		// other values can be returned by some implementations
		if ( ( address )
			&& ( address != reinterpret_cast<void*>( 1 ) ) && ( address != reinterpret_cast<void*>( 2 ) )
			&& ( address != reinterpret_cast<void*>( 3 ) ) && ( address != reinterpret_cast<void*>( -1 ) ) )
		{
			return address;
		}
		std::string wglErrorMessage;
		if ( !address )
		{
			wglErrorMessage = GE3D::Windows::GetLastSystemError();
			GE3D_ASSERTF( false, "The OpenGL extension function \"%s\" wasn't found"
				" (it will now be looked for in the non-extension Windows functions)", i_functionName );
		}
		// wglGetProcAddress() won't return the address of any 1.1 or earlier OpenGL functions
		// that are built into Windows' Opengl32.dll
		// but an attempt can be made to load those manually
		// in case the user of this function has made a mistake
		{
			// This library should already be loaded,
			// and so this function will just retrieve a handle to it
			const auto glLibrary = LoadLibraryW( L"Opengl32.dll" );
			if ( glLibrary != NULL )
			{
				// Look for an old OpenGL function
				const auto* const address = static_cast<void*>( GetProcAddress( glLibrary, i_functionName ) );
				// Decrement the library's reference count
				FreeLibrary( glLibrary );
				// Return an address if it was found
				if ( address )
				{
					return address;
				}
				else
				{
					const auto windowsErrorMessage = GE3D::Windows::GetLastSystemError();
					if ( wglErrorMessage.empty() )
					{
						wglErrorMessage = windowsErrorMessage;
					}
				}
			}
			else
			{
				GE3D_ASSERT( false );
			}
		}

		// If this code is reached the OpenGL function wasn't found
		GE3D_ASSERTF( false, "Couldn't find the address of the OpenGL function \"%s\": %s", i_functionName,
			!wglErrorMessage.empty() ? wglErrorMessage.c_str() : "Unknown error" );
		if ( o_errorMessage )
		{
			std::ostringstream errorMessage;
			errorMessage << "Windows failed to find the address of the OpenGL function \"" << i_functionName << "\"";
			if ( !wglErrorMessage.empty() )
			{
				errorMessage << ": " << wglErrorMessage;
			}
			*o_errorMessage = errorMessage.str();
		}

		return nullptr;
	}
}
