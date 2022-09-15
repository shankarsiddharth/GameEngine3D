// Includes
//=========

#include "../cView.h"
#include "../sContext.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>

eae6320::Graphics::cView::~cView()
{

}

eae6320::cResult eae6320::Graphics::cView::Initialize(const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight)
{
	auto result = eae6320::Results::Success;

	return result;
}

eae6320::cResult eae6320::Graphics::cView::CleanUp()
{
	auto result = Results::Success;
	
	return result;
}

void eae6320::Graphics::cView::Clear(float i_red /*= 0.0f*/, float i_green /*= 0.0f*/, float i_blue /*= 0.0f*/, float i_alpha /*= 1.0f*/)
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
			glClear(clearColor);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
	// In addition to the color buffer there is also a hidden image called the "depth buffer"
	// which is used to make it less important which order draw calls are made.
	// It must also be "cleared" every frame just like the visible color buffer.
	{
		{
			glDepthMask(GL_TRUE);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
			constexpr GLclampd clearToFarDepth = 1.0;
			glClearDepth(clearToFarDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		{
			constexpr GLbitfield clearDepth = GL_DEPTH_BUFFER_BIT;
			glClear(clearDepth);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
}

void eae6320::Graphics::cView::Swap()
{
	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (or "swapped" with the "front buffer", which is the image that is actually being displayed)
	{
		const auto deviceContext = sContext::g_context.deviceContext;
		EAE6320_ASSERT(deviceContext != NULL);
		const auto glResult = SwapBuffers(deviceContext);
		EAE6320_ASSERT(glResult != FALSE);
	}
}
