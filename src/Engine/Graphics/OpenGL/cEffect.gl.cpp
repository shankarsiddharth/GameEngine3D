// Includes
//=========

#include "../cEffect.h"
#include "Includes.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>


void GE3D::Graphics::cEffect::Bind()
{
	// Bind the shading data
	{
		{
			GE3D_ASSERT(m_programId != 0);
			glUseProgram(m_programId);
			GE3D_ASSERT(glGetError() == GL_NO_ERROR);
		}
		// Render state
		{
			m_renderState.Bind();
		}
	}
}

GE3D::cResult GE3D::Graphics::cEffect::InitializeProgram()
{
	auto result = GE3D::Results::Success;

	// Create a program
	GE3D::cScopeGuard scopeGuard_program([&result, this]
		{
			if (!result)
			{
				if (m_programId != 0)
				{
					glDeleteProgram(m_programId);
					const auto errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
						GE3D::Logging::OutputError("OpenGL failed to delete the program: %s",
							reinterpret_cast<const char*>(gluErrorString(errorCode)));
					}
					m_programId = 0;
				}
			}
		});
	{
		GE3D_ASSERT(m_programId == 0);
		m_programId = glCreateProgram();
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to create a program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
		else if (m_programId == 0)
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERT(false);
			GE3D::Logging::OutputError("OpenGL failed to create a program");
			return result;
		}
	}
	// Attach the shaders to the program
	{
		// Vertex
		{
			GE3D_ASSERT((m_vertexShader != nullptr) && (m_vertexShader->m_shaderId != 0));
			glAttachShader(m_programId, m_vertexShader->m_shaderId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = GE3D::Results::Failure;
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				GE3D::Logging::OutputError("OpenGL failed to attach the vertex shader to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		// Fragment
		{
			GE3D_ASSERT((m_fragmentShader != nullptr) && (m_fragmentShader->m_shaderId != 0));
			glAttachShader(m_programId, m_fragmentShader->m_shaderId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = GE3D::Results::Failure;
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				GE3D::Logging::OutputError("OpenGL failed to attach the fragment shader to the program: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
	}
	// Link the program
	{
		glLinkProgram(m_programId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get link info
			// (this won't be used unless linking fails
			// but it can be useful to look at when debugging)
			std::string linkInfo;
			{
				GLint infoSize;
				glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoSize);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (infoSize > 0)
					{
						auto* const info = new (std::nothrow) GLchar[infoSize];
						if (info)
						{
							GE3D::cScopeGuard scopeGuard_info([info]
								{
									delete[] info;
								});
							constexpr GLsizei* const dontReturnLength = nullptr;
							glGetProgramInfoLog(m_programId, static_cast<GLsizei>(infoSize), dontReturnLength, info);
							const auto errorCode = glGetError();
							if (errorCode == GL_NO_ERROR)
							{
								linkInfo = info;
							}
							else
							{
								result = GE3D::Results::Failure;
								GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
								GE3D::Logging::OutputError("OpenGL failed to get link info of the program: %s",
									reinterpret_cast<const char*>(gluErrorString(errorCode)));
								return result;
							}
						}
						else
						{
							result = GE3D::Results::OutOfMemory;
							GE3D_ASSERTF(false, "Couldn't allocate memory for the program link info");
							GE3D::Logging::OutputError("Failed to allocate memory for the program link info");
							return result;
						}
					}
				}
				else
				{
					result = GE3D::Results::Failure;
					GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					GE3D::Logging::OutputError("OpenGL failed to get the length of the program link info: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			// Check to see if there were link errors
			GLint didLinkingSucceed;
			{
				glGetProgramiv(m_programId, GL_LINK_STATUS, &didLinkingSucceed);
				const auto errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didLinkingSucceed == GL_FALSE)
					{
						result = GE3D::Results::Failure;
						GE3D_ASSERTF(false, linkInfo.c_str());
						GE3D::Logging::OutputError("The program failed to link: %s",
							linkInfo.c_str());
						return result;
					}
				}
				else
				{
					result = GE3D::Results::Failure;
					GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					GE3D::Logging::OutputError("OpenGL failed to find out if linking of the program succeeded: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
		}
		else
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to link the program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}

	return result;
}

GE3D::cResult GE3D::Graphics::cEffect::CleanUpProgram()
{
	auto result = GE3D::Results::Success;

	if (m_programId != 0)
	{
		glDeleteProgram(m_programId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			if (result)
			{
				result = GE3D::Results::Failure;
			}
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to delete the program: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_programId = 0;
	}

	return result;
}