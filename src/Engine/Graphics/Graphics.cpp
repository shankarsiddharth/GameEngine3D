// Includes
//=========

#include "Graphics.h"
#include "sContext.h"

#include "cMesh.h"
#include "cEffect.h"
#include "cView.h"
#include "MeshEffectPair.h"

#include "DearImGui.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cEvent.h>
#include <Engine/Logging/Logging.h>
#include <Engine/UserOutput/UserOutput.h>

// Static Data
//============

namespace
{
	std::function<void()> OnImGuiRenderUI = nullptr;

	// View Data
	//--------------

	GE3D::Graphics::cView* s_View = new GE3D::Graphics::cView();

	// Constant buffer object
	GE3D::Graphics::cConstantBuffer s_constantBuffer_frame(GE3D::Graphics::ConstantBufferTypes::Frame);

	GE3D::Graphics::cConstantBuffer s_constantBuffer_drawCall(GE3D::Graphics::ConstantBufferTypes::DrawCall);

	// Submission Data
	//----------------

	// This struct's data is populated at submission time;
	// it must cache whatever is necessary in order to render a frame
	struct sDataRequiredToRenderAFrame
	{
		GE3D::Graphics::ConstantBufferFormats::sFrame constantData_frame;
		float clearColorRed = 0.0f;
		float clearColorGreen = 0.0f;
		float clearColorBlue = 0.0f;
		float clearColorAlpha = 1.0f;
		size_t numberOfPairsToRender = 0;
		GE3D::Graphics::MeshEffectPair meshEffectPair[std::numeric_limits<uint16_t>::max()];
	};
	// In our class there will be two copies of the data required to render a frame:
	//	* One of them will be in the process of being populated by the data currently being submitted by the application loop thread
	//	* One of them will be fully populated and in the process of being rendered from in the render thread
	// (In other words, one is being produced while the other is being consumed)
	sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
	auto* s_dataBeingSubmittedByApplicationThread = &s_dataRequiredToRenderAFrame[0];
	auto* s_dataBeingRenderedByRenderThread = &s_dataRequiredToRenderAFrame[1];
	// The following two events work together to make sure that
	// the main/render thread and the application loop thread can work in parallel but stay in sync:
	// This event is signaled by the application loop thread when it has finished submitting render data for a frame
	// (the main/render thread waits for the signal)
	GE3D::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
	// This event is signaled by the main/render thread when it has swapped render data pointers.
	// This means that the renderer is now working with all the submitted data it needs to render the next frame,
	// and the application loop thread can start submitting data for the following frame
	// (the application loop thread waits for the signal)
	GE3D::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;

}

// Helper Declarations
//====================

namespace
{
	GE3D::cResult InitializeGeometry();
	GE3D::cResult InitializeShadingData();
	GE3D::cResult InitializeViews(const GE3D::Graphics::sInitializationParameters& i_initializationParameters);
	GE3D::cResult CleanUpRenderData(sDataRequiredToRenderAFrame* i_renderData);
}

void GE3D::Graphics::RegisterOnImGuiRenderUI(const std::function<void()>& i_OnRenderImGuiRenderUI)
{
	OnImGuiRenderUI = i_OnRenderImGuiRenderUI;
}

// Submission
//-----------

void GE3D::Graphics::SubmitElapsedTime(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime)
{
	GE3D_ASSERT(s_dataBeingSubmittedByApplicationThread)
		auto& constantData_frame = s_dataBeingSubmittedByApplicationThread->constantData_frame;
	constantData_frame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_frame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}

GE3D::cResult GE3D::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}

GE3D::cResult GE3D::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}

void GE3D::Graphics::SetBackgroundClearColor(float i_red, float i_green, float i_blue, float i_alpha)
{
	s_dataBeingSubmittedByApplicationThread->clearColorRed = i_red;
	s_dataBeingSubmittedByApplicationThread->clearColorGreen = i_green;
	s_dataBeingSubmittedByApplicationThread->clearColorBlue = i_blue;
	s_dataBeingSubmittedByApplicationThread->clearColorAlpha = i_alpha;
}

void GE3D::Graphics::SubmitMeshEffectPairs(GE3D::Graphics::MeshEffectPair*& i_meshEffectPair, size_t i_numberOfPairsToRender)
{
	size_t cache_index = s_dataBeingSubmittedByApplicationThread->numberOfPairsToRender;

	size_t checkIndex = s_dataBeingSubmittedByApplicationThread->numberOfPairsToRender + i_numberOfPairsToRender;

	GE3D_ASSERT(checkIndex < std::numeric_limits<uint16_t>::max());

	s_dataBeingSubmittedByApplicationThread->numberOfPairsToRender += i_numberOfPairsToRender;

	for (size_t input_index = 0; cache_index < s_dataBeingSubmittedByApplicationThread->numberOfPairsToRender; cache_index++, input_index++)
	{
		s_dataBeingSubmittedByApplicationThread->meshEffectPair[cache_index].mesh = i_meshEffectPair[input_index].mesh;
		s_dataBeingSubmittedByApplicationThread->meshEffectPair[cache_index].mesh->IncrementReferenceCount();
		s_dataBeingSubmittedByApplicationThread->meshEffectPair[cache_index].drawCallData = i_meshEffectPair[input_index].drawCallData;
		s_dataBeingSubmittedByApplicationThread->meshEffectPair[cache_index].effect = i_meshEffectPair[input_index].effect;
		s_dataBeingSubmittedByApplicationThread->meshEffectPair[cache_index].effect->IncrementReferenceCount();
	}

	GE3D_ASSERT(true);
}

void GE3D::Graphics::SubmitCameraTransform(const GE3D::Math::cMatrix_transformation& i_worldToCameraTransform, const GE3D::Math::cMatrix_transformation& i_cameraToProjectedTransform_perspective)
{
	s_dataBeingSubmittedByApplicationThread->constantData_frame.g_transform_worldToCamera = i_worldToCameraTransform;
	s_dataBeingSubmittedByApplicationThread->constantData_frame.g_transform_cameraToProjected = i_cameraToProjectedTransform_perspective;
}

// Render
//-------

void GE3D::Graphics::RenderFrame()
{
	// Wait for the application loop to submit data to be rendered
	{
		if (Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread))
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread, s_dataBeingRenderedByRenderThread);
			// Once the pointers have been swapped the application loop can submit new data
			if (!s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal())
			{
				GE3D_ASSERTF(false, "Couldn't signal that new graphics data can be submitted")
					Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			GE3D_ASSERTF(false, "Waiting for the graphics data to be submitted failed")
				Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
	}

	GE3D::Graphics::DearImGui::CreateImGuiFrame();
	//GE3D::Graphics::DearImGui::RenderImGuiUI();
	if (OnImGuiRenderUI != nullptr)
	{
		OnImGuiRenderUI();
	}
	GE3D::Graphics::DearImGui::RenderImGuiFrame();

	GE3D_ASSERT(s_dataBeingRenderedByRenderThread);
	auto* const dataRequiredToRenderFrame = s_dataBeingRenderedByRenderThread;

	// Clear the Background Color
	s_View->Clear(dataRequiredToRenderFrame->clearColorRed, dataRequiredToRenderFrame->clearColorGreen, dataRequiredToRenderFrame->clearColorBlue, dataRequiredToRenderFrame->clearColorAlpha);

	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		auto& constantData_frame = dataRequiredToRenderFrame->constantData_frame;
		s_constantBuffer_frame.Update(&constantData_frame);
	}

	//Bind Effect & Draw Mesh 
	size_t numberOfPairsToRender = dataRequiredToRenderFrame->numberOfPairsToRender;
	for (size_t index = 0; index < numberOfPairsToRender; index++)
	{
		if (dataRequiredToRenderFrame->meshEffectPair[index].mesh != nullptr 
			&& dataRequiredToRenderFrame->meshEffectPair[index].effect != nullptr)
		{
			dataRequiredToRenderFrame->meshEffectPair[index].effect->Bind();			
			// Update the draw call constant buffer
			{
				// Copy the data from the system memory that the application owns to GPU memory
				//GE3D::Graphics::ConstantBufferFormats::sDrawCall constantData_drawCall;
				//constantData_drawCall.g_transform_localToWorld = dataRequiredToRenderFrame->meshEffectPair[index].mesh->GetTransform();
				auto& constantData_drawCall = dataRequiredToRenderFrame->meshEffectPair[index].drawCallData;
				s_constantBuffer_drawCall.Update(&constantData_drawCall);
			}
			dataRequiredToRenderFrame->meshEffectPair[index].mesh->Draw();
		}
	}	
	
	GE3D::Graphics::DearImGui::RenderImGui_DrawData();

	s_View->Swap();

	// After all of the data that was submitted for this frame has been used
	// you must make sure that it is all cleaned up and cleared out
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	{
		// (At this point in the class there isn't anything that needs to be cleaned up)
		//dataRequiredToRenderFrame	// TODO
		auto result = GE3D::Results::Success;
		result = CleanUpRenderData(dataRequiredToRenderFrame);
	}
}

// Initialize / Clean Up
//----------------------

GE3D::cResult GE3D::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		GE3D_ASSERTF(false, "Can't initialize Graphics without context");
		return result;
	}
	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_frame.Initialize())
		{
			// There is only a single frame constant buffer that is reused
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_frame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			GE3D_ASSERTF(false, "Can't initialize Graphics without frame constant buffer");
			return result;
		}

		if (result = s_constantBuffer_drawCall.Initialize())
		{
			s_constantBuffer_drawCall.Bind(
				static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			GE3D_ASSERTF(false, "Can't initialize Graphics without draw call constant buffer");
			return result;
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled)))
		{
			GE3D_ASSERTF(false, "Can't initialize Graphics without event for when data has been submitted from the application thread");
			return result;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			GE3D_ASSERTF(false, "Can't initialize Graphics without event for when data can be submitted from the application thread");
			return result;
		}
	}
	// Initialize the views
	{
		if (!(result = InitializeViews(i_initializationParameters)))
		{
			GE3D_ASSERTF(false, "Can't initialize Graphics without the views");
			return result;
		}
	}
	
	// Initialize the Dear ImGui
	{
		if (!(result = GE3D::Graphics::DearImGui::InitializeImGui(i_initializationParameters)))
		{
			GE3D_ASSERTF(false, "Can't initialize ImGui");
			return result;
		}
	}

	// Initialize the shading data
	{
		if (!(result = InitializeShadingData()))
		{
			GE3D_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	// Initialize the geometry
	{
		if (!(result = InitializeGeometry()))
		{
			GE3D_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}

	//Size Information:
	size_t sizeOfRenderData = sizeof(sDataRequiredToRenderAFrame);
	std::string gPlatformName;
#if defined( GE3D_PLATFORM_WINDOWS )	
#if defined( GE3D_PLATFORM_D3D )
	gPlatformName = "Direct3D";
#elif defined( GE3D_PLATFORM_GL )
	gPlatformName = "OpenGL";
#endif
#endif
	GE3D::Logging::OutputMessage("Size of Render Data in  %s  using sizeof operator : %zu bytes", gPlatformName.c_str(), (sizeOfRenderData * 2));

	size_t sizeOfDrawCallData = sizeof(GE3D::Graphics::ConstantBufferFormats::sDrawCall);
	GE3D::Logging::OutputMessage("Size of Draw Call Cache Data for a Mesh in  %s  using sizeof operator : %zu bytes", gPlatformName.c_str(), (sizeOfDrawCallData));

	return result;
}

GE3D::cResult GE3D::Graphics::CleanUp()
{
	auto result = Results::Success;

	result = s_View->CleanUp();

	//Cleanup Render Data
	result = CleanUpRenderData(s_dataBeingRenderedByRenderThread);
	result = CleanUpRenderData(s_dataBeingSubmittedByApplicationThread);

	{
		const auto result_constantBuffer_frame = s_constantBuffer_frame.CleanUp();
		if (!result_constantBuffer_frame)
		{
			GE3D_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_frame;
			}
		}
	}

	{
		const auto result_constantBuffer_drawCall = s_constantBuffer_drawCall.CleanUp();
		if (!result_constantBuffer_drawCall)
		{
			GE3D_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_drawCall;
			}
		}
	}

	{
		const auto result_context = sContext::g_context.CleanUp();
		if (!result_context)
		{
			GE3D_ASSERT(false);
			if (result)
			{
				result = result_context;
			}
		}
	}

	//result = GE3D::Graphics::DearImGui::CleanUpImGui();

	return result;
}


// Helper Definitions
//===================

namespace
{
	GE3D::cResult InitializeGeometry()
	{
		auto result = GE3D::Results::Success;

		GE3D::Graphics::VertexFormats::sVertex_mesh vertexData[] =
		{
			// Direct3D is left-handed
			{0.0f, 0.0f, 0.0f},
			{1.0f, 1.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}
		};

		uint16_t indexData[] =
		{
			// Direct3D is left-handed
			0, 1, 2, 0, 3, 1
		};

		//result = GE3D::Graphics::cMesh::CreateMesh(vertexData, 4, indexData, 6, s_newMesh);

		GE3D::Graphics::VertexFormats::sVertex_mesh newVertexData[] =
		{
			// Direct3D is left-handed
			{0.0f, 0.0f, 0.0f},
			{-1.0f, -1.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f}
		};

		uint16_t newIndexData[] =
		{
			// Direct3D is left-handed
			0, 1, 2
		};

		//result = GE3D::Graphics::cMesh::CreateMesh(newVertexData, 3, newIndexData, 3, s_secondMesh);

		return result;
	}

	GE3D::cResult InitializeShadingData()
	{
		auto result = GE3D::Results::Success;

		//result = GE3D::Graphics::cEffect::CreateEffect(s_newEffect, "data/Shaders/Fragment/testsample.bsasset");

		//result = GE3D::Graphics::cEffect::CreateEffect(s_secondEffect);

		return result;
	}

	GE3D::cResult InitializeViews(const GE3D::Graphics::sInitializationParameters& i_initializationParameters)
	{
		auto result = GE3D::Results::Success;

		unsigned int resolutionWidth = 0;
		unsigned int resolutionHeight = 0;

#if defined( GE3D_PLATFORM_WINDOWS )
#if defined( GE3D_PLATFORM_D3D )
		resolutionWidth = i_initializationParameters.resolutionWidth;
		resolutionHeight = i_initializationParameters.resolutionHeight;
#endif
#endif
		result = s_View->Initialize(resolutionWidth, resolutionHeight);

		return result;
	}

	GE3D::cResult CleanUpRenderData(sDataRequiredToRenderAFrame* i_renderData)
	{
		auto result = GE3D::Results::Success;

		size_t numberOfPairsToRender = i_renderData->numberOfPairsToRender;
		for (size_t index = 0; index < numberOfPairsToRender; index++)
		{
			if (i_renderData->meshEffectPair[index].mesh != nullptr
				&& i_renderData->meshEffectPair[index].effect != nullptr)
			{
				i_renderData->meshEffectPair[index].mesh->DecrementReferenceCount();
				i_renderData->meshEffectPair[index].mesh = nullptr;
				i_renderData->meshEffectPair[index].effect->DecrementReferenceCount();
				i_renderData->meshEffectPair[index].effect = nullptr;
			}
		}

		i_renderData->numberOfPairsToRender = 0;

		return result;
	}
}