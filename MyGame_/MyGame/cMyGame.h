/*
	This class is your specific game
*/

#ifndef EAE6320_CMYGAME_H
#define EAE6320_CMYGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>
#include <Engine/Camera/cCamera.h>
#include <Engine/GameFramework/cGameObject.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
#include "Resource Files/Resource.h"
#endif

// Forward Declaration
//==================

namespace eae6320 
{
	namespace Graphics
	{
		class cMesh;
		class cEffect;
		struct MeshEffectPair;
	}
}


// Class Declaration
//==================

namespace eae6320
{
	class cMyGame final : public Application::iApplication
	{
		// Inherited Implementation
		//=========================

		void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime,
			const float i_elapsedSecondCount_sinceLastSimulationUpdate) override;

		void UpdateSimulationBasedOnInput() override;

		void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

	private:

		eae6320::Graphics::cMesh* m_planeMesh = nullptr;
		eae6320::Graphics::cMesh* m_sphereMesh = nullptr;
		eae6320::Graphics::cMesh* m_sphereLargeMesh = nullptr;
		eae6320::Graphics::cMesh* m_helixMesh = nullptr;
		eae6320::Graphics::cEffect* m_animColorEffect = nullptr;
		eae6320::Graphics::cEffect* m_defaultEffect = nullptr;


		eae6320::Camera::cCamera* m_camera = nullptr;

		eae6320::GameFramework::cGameObject* m_sphereGameObject = nullptr;
		eae6320::GameFramework::cGameObject* m_planeGameObject = nullptr;
		eae6320::GameFramework::cGameObject* m_helixGameObject = nullptr;

		static const size_t MAXIMUM_NUMBER_OF_PAIRS = 500;
		static size_t s_numberOfPairsToRender;
		eae6320::Graphics::MeshEffectPair* m_meshEffectPair = nullptr;

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "Sample EAE6320 MyGame"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#else
				" -- Release"
#endif
				;
		}

		const char* GetPlatformName() const;

		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "Sample EAE6320 MyGame Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_eae_alien = IDI_EAEALIEN; return &iconId_eae_alien; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_eae_game_pad = IDI_EAEGAMEPAD; return &iconId_eae_game_pad; }
#endif

		// Run
		//----

		void UpdateBasedOnInput() final;

		// Initialize / Clean Up
		//----------------------

		cResult Initialize() final;
		cResult CleanUp() final;


		eae6320::cResult InitializeGeometry();
		eae6320::cResult InitializeShadingData();
		eae6320::cResult InitializePairs();
		eae6320::cResult InitializeGameObjects();
		eae6320::cResult InitializeStory();
				
	};
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Application
		{
			// You can add specific results for your game here:
			//	* The System should always be Application
			//	* The __LINE__ macro is used to make sure that every result has a unique ID.
			//		That means, however, that all results _must_ be defined in this single file
			//		or else you could have two different ones with equal IDs.
			//	* Note that you can define multiple Success codes.
			//		This can be used if the caller may want to know more about how a function succeeded.
			constexpr cResult ExampleResult(IsFailure, eSystem::Application, __LINE__, Severity::Default);
		}
	}
}

#endif	// EAE6320_CMYGAME_H
