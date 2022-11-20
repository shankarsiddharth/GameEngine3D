/*
	This file declares the external interface for the graphics system
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Includes
//=========

#include "Configuration.h"
#include "InitializationParameters.h"

#include <Engine/Results/Results.h>
#include <Engine/Math/cMatrix_transformation.h>

// Forward Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		struct MeshEffectPair;
	}
}

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{

		// Submission
		//-----------

		// These functions should be called from the application (on the application loop thread)

		// As the class progresses you will add your own functions for submitting data,
		// but the following is an example (that gets called automatically)
		// of how the application submits the total elapsed times
		// for the frame currently being submitted
		void SubmitElapsedTime( const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_simulationTime );

		// When the application is ready to submit data for a new frame
		// it should call this before submitting anything
		// (or, said another way, it is not safe to submit data for a new frame
		// until this function returns successfully)
		cResult WaitUntilDataForANewFrameCanBeSubmitted( const unsigned int i_timeToWait_inMilliseconds );
		// When the application has finished submitting data for a frame
		// it must call this function
		cResult SignalThatAllDataForAFrameHasBeenSubmitted();

		void SetBackgroundClearColor(float i_red = 0.0f, float i_green = 0.0f, float i_blue = 0.0f, float i_alpha = 1.0f);

		void SubmitMeshEffectPairs(eae6320::Graphics::MeshEffectPair*& i_meshEffectPair, size_t i_numberOfPairsToRender);

		void SubmitCameraTransform(const eae6320::Math::cMatrix_transformation& i_worldToCameraTransform, const eae6320::Math::cMatrix_transformation& i_cameraToProjectedTransform_perspective);

		// Render
		//-------

		// This is called (automatically) from the main/render thread.
		// It will render a submitted frame as soon as it is ready
		// (i.e. as soon as SignalThatAllDataForAFrameHasBeenSubmitted() has been called)
		void RenderFrame();

		// Initialize / Clean Up
		//----------------------

		cResult Initialize( const sInitializationParameters& i_initializationParameters );
		cResult CleanUp();
	}
}

#endif	// EAE6320_GRAPHICS_H
