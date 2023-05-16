// Includes
//=========

#include "cEffect.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>


GE3D::cResult GE3D::Graphics::cEffect::CreateEffect(cEffect*& o_effect, const std::string& i_fragmentShaderPath, const std::string& i_vertexShaderPath)
{
	auto result = Results::Success;

	GE3D_ASSERT(o_effect == nullptr);

	cEffect* newEffect = nullptr;
	cScopeGuard scopeGuard([&o_effect, &result, &newEffect]
	{
		if (result)
		{
			GE3D_ASSERT(newEffect != nullptr);
			o_effect = newEffect;
		}
		else
		{
			if (newEffect)
			{
				newEffect->DecrementReferenceCount();
				newEffect = nullptr;
			}
			o_effect = nullptr;
		}
	});

	// Allocate a new Effect
	{
		newEffect = new (std::nothrow) cEffect();
		if (!newEffect)
		{
			result = Results::OutOfMemory;
			GE3D_ASSERTF(false, "Couldn't allocate memory for a effect");
			Logging::OutputError("Failed to allocate memory for a effect");
			return result;
		}
	}
	// Initialize the platform-specific effect
	if (!(result = newEffect->Initialize(i_fragmentShaderPath, i_vertexShaderPath)))
	{
		GE3D_ASSERTF(false, "Initialization of new effect failed");
		return result;
	}

	return result;
}

GE3D::Graphics::cEffect::~cEffect()
{
	GE3D_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	GE3D_ASSERT(result);
}

GE3D::cResult GE3D::Graphics::cEffect::Initialize(const std::string& i_fragmentShaderPath/* = ""*/, const std::string& i_vertexShaderPath/* = ""*/)
{
	auto result = GE3D::Results::Success;

	std::string fragmentShaderPath = i_fragmentShaderPath;
	std::string vertexShaderPath = i_vertexShaderPath;

	if (i_vertexShaderPath.empty())
	{
		vertexShaderPath = "data/Shaders/Vertex/standard.bsasset";
	}
	if (i_fragmentShaderPath.empty())
	{
		fragmentShaderPath = "data/Shaders/Fragment/standard.bsasset";
	}

	if (!(result = GE3D::Graphics::cShader::Load(vertexShaderPath,
		m_vertexShader, GE3D::Graphics::eShaderType::Vertex)))
	{
		GE3D_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = GE3D::Graphics::cShader::Load(fragmentShaderPath,
		m_fragmentShader, GE3D::Graphics::eShaderType::Fragment)))
	{
		GE3D_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return result;
	}
	{
		constexpr auto renderStateBits = []
		{
			uint8_t renderStateBits = 0;

			GE3D::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
			/*GE3D::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
			GE3D::Graphics::RenderStates::DisableDepthWriting(renderStateBits);*/
			GE3D::Graphics::RenderStates::EnableDepthTesting(renderStateBits);
			GE3D::Graphics::RenderStates::EnableDepthWriting(renderStateBits);
			GE3D::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

			return renderStateBits;
		}();
		if (!(result = m_renderState.Initialize(renderStateBits)))
		{
			GE3D_ASSERTF(false, "Can't initialize shading data without render state");
			return result;
		}
	}

#if defined( GE3D_PLATFORM_WINDOWS )			
#if defined( GE3D_PLATFORM_D3D )			
#elif defined( GE3D_PLATFORM_GL )
	result = InitializeProgram();
#endif
#endif

	return result;
}

GE3D::cResult GE3D::Graphics::cEffect::CleanUp()
{
	auto result = Results::Success;
	
#if defined( GE3D_PLATFORM_WINDOWS )			
#if defined( GE3D_PLATFORM_D3D )			
#elif defined( GE3D_PLATFORM_GL )
	result = CleanUpProgram();
#endif
#endif

	if (m_vertexShader)
	{
		m_vertexShader->DecrementReferenceCount();
		m_vertexShader = nullptr;
	}
	if (m_fragmentShader)
	{
		m_fragmentShader->DecrementReferenceCount();
		m_fragmentShader = nullptr;
	}

	return result;
}
