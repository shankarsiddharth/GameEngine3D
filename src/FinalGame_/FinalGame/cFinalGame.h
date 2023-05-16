/*
	This class is your specific game
*/

#ifndef GE3D_CFINALGAME_H
#define GE3D_CFINALGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Results/Results.h>
#include "Engine/StoryNarrator/Story/Story.h"
#include <Engine/Audio/AudioSystem.h>
#include <External/imgui/Includes.h>

#include <Engine/Camera/cCamera.h>
#include <Engine/GameFramework/cGameObject.h>

#if defined( GE3D_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif



// Class Declaration
//==================

namespace GE3D
{
	class cFinalGame final : public Application::iApplication
	{
		// Inherited Implementation
		//=========================

		void SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime,
			const float i_elapsedSecondCount_sinceLastSimulationUpdate) override;

		void UpdateSimulationBasedOnInput() override;

		void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;

		void RenderUI();

	private:

		// 3D Objects
		//--------------
		GE3D::Graphics::cMesh* m_planeMesh = nullptr;
		GE3D::Graphics::cMesh* m_sonicMesh = nullptr;
		GE3D::Graphics::cMesh* m_sphereLargeMesh = nullptr;
		GE3D::Graphics::cMesh* m_helixMesh = nullptr;
		GE3D::Graphics::cEffect* m_animColorEffect = nullptr;
		GE3D::Graphics::cEffect* m_defaultEffect = nullptr;


		GE3D::Camera::cCamera* m_camera = nullptr;

		GE3D::GameFramework::cGameObject* m_sonicGameObject = nullptr;
		GE3D::GameFramework::cGameObject* m_planeGameObject = nullptr;
		GE3D::GameFramework::cGameObject* m_helixGameObject = nullptr;

		static const size_t MAXIMUM_NUMBER_OF_PAIRS = 500;
		static size_t s_numberOfPairsToRender;
		GE3D::Graphics::MeshEffectPair* m_meshEffectPair = nullptr;


		// ImGui Members
		//--------------
		ImFont* m_latoFont = nullptr;
		ImFont* m_defaultFont = nullptr;
		ImGuiWindowFlags window_flags = 0;

		std::string story_string;
		Narrator::Runtime::Story story;
		bool m_showStartStory = true;
		bool m_displayChoices = false;
		bool m_choiceSelected = false;
		size_t m_currentChoiceCount = 0;
		size_t m_selectedChoice = 0;
		bool m_enableScrollTrack = true;
		
		bool m_isEndOfStory = false;

		bool m_ShowImGuiUI = true;
		bool m_ShowViewportUI = false;

		GE3D::AudioSystem::cAudio m_BGAudio;
		GE3D::AudioSystem::cAudio m_SelectAudio;

		// Configuration
		//--------------

#if defined( GE3D_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "GE3D FinalGame"
				" -- "
#if defined( GE3D_PLATFORM_D3D )
				"Direct3D"
#elif defined( GE3D_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
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
		const char* GetMainWindowClassName() const final { return "GE3D FinalGame Window Class"; }
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

		// ImGui Methods
		//--------------
		GE3D::cResult InitializeUI();

		GE3D::cResult InitializeStory();
		void ReadStory();

		GE3D::cResult InitializeAudio();

		// 3D Objects
		//--------------
		GE3D::cResult InitializeGeometry();
		GE3D::cResult InitializeShadingData();
		GE3D::cResult InitializePairs();
		GE3D::cResult InitializeGameObjects();
		
		void GetDefaultInitialResolution(uint16_t& o_width, uint16_t& o_height) const override;
		
	};
}

// Result Definitions
//===================

namespace GE3D
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
			constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
		}
	}
}

#endif	// GE3D_CFINALGAME_H
