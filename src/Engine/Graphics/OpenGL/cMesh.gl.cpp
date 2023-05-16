// Includes
//=========

#include "../cMesh.h"
#include "../sContext.h"
#include "../VertexFormats.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>

GE3D::cResult GE3D::Graphics::cMesh::Initialize(GE3D::Graphics::VertexFormats::sVertex_mesh* i_vertexArray, size_t i_vertexArraySize, uint16_t* i_indexArray, size_t i_indexArraySize)
{
	auto result = GE3D::Results::Success;

	// Create a vertex array object and make it active
	{
		constexpr GLsizei arrayCount = 1;
		glGenVertexArrays(arrayCount, &m_vertexArrayId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindVertexArray(m_vertexArrayId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = GE3D::Results::Failure;
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				GE3D::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Create a vertex buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_vertexBufferId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = GE3D::Results::Failure;
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				GE3D::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Assign the data to the vertex buffer
	{
		GE3D_ASSERT(i_vertexArraySize != 0)
		//constexpr unsigned int triangleCount = 2;
		//constexpr unsigned int vertexCountPerTriangle = 3;
		const auto vertexCount = i_vertexArraySize;
		const auto bufferSize = sizeof(i_vertexArray[0]) * vertexCount;
		GE3D_ASSERT(static_cast<GLsizeiptr>(bufferSize) <= std::numeric_limits<GLsizeiptr>::max());
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(i_vertexArray),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Create a index buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_indexBufferId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = GE3D::Results::Failure;
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				GE3D::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Assign the data to the index buffer
	{
		constexpr unsigned int indexCountPerTriangle = 3;		
		GE3D_ASSERT(i_indexArraySize != 0)
		GE3D_ASSERT(i_indexArraySize % indexCountPerTriangle == 0)
		m_indexBufferSize = i_indexArraySize;		
		const auto bufferSize = sizeof(i_indexArray[0]) * m_indexBufferSize;
		GE3D_ASSERT(static_cast<GLsizeiptr>(bufferSize) <= std::numeric_limits<GLsizeiptr>::max());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(i_indexArray),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = GE3D::Results::Failure;
			GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			GE3D::Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Initialize vertex format
	{
		// The "stride" defines how large a single vertex is in the stream of data
		// (or, said another way, how far apart each position element is)
		constexpr auto stride = static_cast<GLsizei>(sizeof(GE3D::Graphics::VertexFormats::sVertex_mesh));

		// Position (0)
		// 3 floats == 12 bytes
		// Offset = 0
		{
			constexpr GLuint vertexElementLocation = 0;
			constexpr GLint elementCount = 3;
			constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(GE3D::Graphics::VertexFormats::sVertex_mesh, x)));
			const auto errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = GE3D::Results::Failure;
					GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					GE3D::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			else
			{
				result = GE3D::Results::Failure;
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				GE3D::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}

		// Color (1)
		// 4 8-bit ints == 4 bytes
		{
			constexpr GLuint vertexElementLocation = 1;
			constexpr GLint elementCount = 4;
			constexpr GLboolean normalized = GL_TRUE;	// GPU to treat [0,255] as [0,1]
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_UNSIGNED_BYTE, normalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(GE3D::Graphics::VertexFormats::sVertex_mesh, r)));
			const auto errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = GE3D::Results::Failure;
					GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					GE3D::Logging::OutputError("OpenGL failed to enable the COLOR vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			else
			{
				result = GE3D::Results::Failure;
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				GE3D::Logging::OutputError("OpenGL failed to set the COLOR vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
	}
	
	return result;
}

GE3D::cResult GE3D::Graphics::cMesh::CleanUp()
{
	auto result = Results::Success;

	{
		if (m_vertexArrayId != 0)
		{
			// Make sure that the vertex array isn't bound
			{
				// Unbind the vertex array
				glBindVertexArray(0);
				const auto errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					if (result)
					{
						result = Results::Failure;
					}
					GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
			}
			constexpr GLsizei arrayCount = 1;
			glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				if (result)
				{
					result = Results::Failure;
				}
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to delete the vertex array: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
			m_vertexArrayId = 0;
		}
		if (m_indexBufferId != 0)
		{
			constexpr GLsizei bufferCount = 1;
			glDeleteBuffers(bufferCount, &m_indexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				if (result)
				{
					result = Results::Failure;
				}
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to delete the index buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
			m_indexBufferId = 0;
		}
		if (m_vertexBufferId != 0)
		{
			constexpr GLsizei bufferCount = 1;
			glDeleteBuffers(bufferCount, &m_vertexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				if (result)
				{
					result = Results::Failure;
				}
				GE3D_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
			m_vertexBufferId = 0;
		}
	}

	return result;
}

void GE3D::Graphics::cMesh::Draw()
{
	// Draw the geometry
	{
		// Bind a specific vertex buffer to the device as a data source
		{
			GE3D_ASSERT(m_vertexArrayId != 0);
			glBindVertexArray(m_vertexArrayId);
			GE3D_ASSERT(glGetError() == GL_NO_ERROR);
		}
		//// Render triangles from the currently-bound vertex buffer
		//{
		//	// The mode defines how to interpret multiple vertices as a single "primitive";
		//	// a triangle list is defined
		//	// (meaning that every primitive is a triangle and will be defined by three vertices)
		//	constexpr GLenum mode = GL_TRIANGLES;
		//	// As of this comment only a single triangle is drawn
		//	// (you will have to update this code in future assignments!)
		//	constexpr unsigned int triangleCount = 2;
		//	constexpr unsigned int vertexCountPerTriangle = 3;
		//	constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
		//	// It's possible to start rendering primitives in the middle of the stream
		//	constexpr unsigned int indexOfFirstVertexToRender = 0;
		//	glDrawArrays(mode, indexOfFirstVertexToRender, vertexCountToRender);
		//	GE3D_ASSERT(glGetError() == GL_NO_ERROR);
		//}
		// Render triangles from the currently-bound vertex buffer
		{
			// The mode defines how to interpret multiple vertices as a single "primitive";
			// a triangle list is defined
			// (meaning that every primitive is a triangle and will be defined by three vertices)
			constexpr GLenum mode = GL_TRIANGLES;			
			// It's possible to start rendering primitives in the middle of the stream
			const GLvoid* const offset = 0;
			glDrawElements(mode, static_cast<GLsizei>(m_indexBufferSize), GL_UNSIGNED_SHORT, offset);
			GE3D_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
}
