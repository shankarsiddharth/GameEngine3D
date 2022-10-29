// Includes
//=========

#include "cEffect.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>


eae6320::cResult eae6320::Graphics::cEffect::CreateEffect(cEffect*& o_effect, const std::string& i_fragmentShaderPath, const std::string& i_vertexShaderPath)
{
	auto result = Results::Success;

	EAE6320_ASSERT(o_effect == nullptr);

	cEffect* newEffect = nullptr;
	cScopeGuard scopeGuard([&o_effect, &result, &newEffect]
	{
		if (result)
		{
			EAE6320_ASSERT(newEffect != nullptr);
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
			EAE6320_ASSERTF(false, "Couldn't allocate memory for a effect");
			Logging::OutputError("Failed to allocate memory for a effect");
			return result;
		}
	}
	// Initialize the platform-specific effect
	if (!(result = newEffect->Initialize(i_fragmentShaderPath, i_vertexShaderPath)))
	{
		EAE6320_ASSERTF(false, "Initialization of new effect failed");
		return result;
	}

	return result;
}

eae6320::Graphics::cEffect::~cEffect()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}

eae6320::cResult eae6320::Graphics::cEffect::Initialize(const std::string& i_fragmentShaderPath/* = ""*/, const std::string& i_vertexShaderPath/* = ""*/)
{
	auto result = eae6320::Results::Success;

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

	if (!(result = eae6320::Graphics::cShader::Load(vertexShaderPath,
		m_vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = eae6320::Graphics::cShader::Load(fragmentShaderPath,
		m_fragmentShader, eae6320::Graphics::eShaderType::Fragment)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return result;
	}
	{
		constexpr auto renderStateBits = []
		{
			uint8_t renderStateBits = 0;

			eae6320::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
			/*eae6320::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthWriting(renderStateBits);*/
			eae6320::Graphics::RenderStates::EnableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::EnableDepthWriting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

			return renderStateBits;
		}();
		if (!(result = m_renderState.Initialize(renderStateBits)))
		{
			EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
			return result;
		}
	}

#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )			
#elif defined( EAE6320_PLATFORM_GL )
	result = InitializeProgram();
#endif
#endif

	return result;
}

eae6320::cResult eae6320::Graphics::cEffect::CleanUp()
{
	auto result = Results::Success;
	
#if defined( EAE6320_PLATFORM_WINDOWS )			
#if defined( EAE6320_PLATFORM_D3D )			
#elif defined( EAE6320_PLATFORM_GL )
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