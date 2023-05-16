// Includes
//=========

#include "../cEffect.h"
#include "../sContext.h"
#include "Includes.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>


void GE3D::Graphics::cEffect::Bind()
{
	auto* const direct3dImmediateContext = sContext::g_context.direct3dImmediateContext;
	GE3D_ASSERT(direct3dImmediateContext);

	// Bind the shading data
	{
		{
			constexpr ID3D11ClassInstance* const* noInterfaces = nullptr;
			constexpr unsigned int interfaceCount = 0;
			// Vertex shader
			{
				GE3D_ASSERT((m_vertexShader != nullptr) && (m_vertexShader->m_shaderObject.vertex != nullptr));
				direct3dImmediateContext->VSSetShader(m_vertexShader->m_shaderObject.vertex, noInterfaces, interfaceCount);
			}
			// Fragment shader
			{
				GE3D_ASSERT((m_fragmentShader != nullptr) && (m_fragmentShader->m_shaderObject.vertex != nullptr));
				direct3dImmediateContext->PSSetShader(m_fragmentShader->m_shaderObject.fragment, noInterfaces, interfaceCount);
			}
		}
		// Render state
		{
			m_renderState.Bind();
		}
	}
}
