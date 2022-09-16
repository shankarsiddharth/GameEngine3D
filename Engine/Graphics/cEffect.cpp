// Includes
//=========

#include "cEffect.h"


eae6320::Graphics::cEffect::~cEffect()
{

}

eae6320::cResult eae6320::Graphics::cEffect::Initialize(const std::string& i_fragmentShaderPath/* = ""*/, const std::string& i_vertexShaderPath/* = ""*/)
{
	auto result = eae6320::Results::Success;

	std::string fragmentShaderPath = i_fragmentShaderPath;
	std::string vertexShaderPath = i_vertexShaderPath;

	if (i_vertexShaderPath.empty())
	{
		vertexShaderPath = "data/Shaders/Vertex/standard.shader";
	}
	if (i_fragmentShaderPath.empty())
	{
		fragmentShaderPath = "data/Shaders/Fragment/standard.shader";
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
			eae6320::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDepthWriting(renderStateBits);
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