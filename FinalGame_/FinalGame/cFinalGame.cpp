// Includes
//=========

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>

#include <Engine/StoryNarrator/Includes.h>
#include <External/imgui/Includes.h>

#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/MeshEffectPair.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>

#include <random>
#include <string>

namespace
{
	struct GameState
	{
		size_t numberOfPairsToDraw = 0;
		bool bShouldSwapEffects = false;
	};

	GameState s_GameState;
}

size_t eae6320::cFinalGame::s_numberOfPairsToRender = 0;

const char* eae6320::cFinalGame::GetPlatformName() const
{
	return
#if defined( EAE6320_PLATFORM_D3D )
		"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
		"OpenGL"
#endif
		//#ifdef _DEBUG
		//		" -- Debug"
		//#else
		//		" -- Release"
		//#endif
		;
}

// Inherited Implementation
//=========================

void eae6320::cFinalGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime,
	const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	m_BGAudio.SubmitAudioSource();
	m_SelectAudio.SubmitAudioSource();

	if (m_ShowImGuiUI && !m_ShowViewportUI)
	{
		return;
	}

	//3D Objects
	//eae6320::Graphics::SetBackgroundClearColor(0.0f, 0.95f, 1.0f);

	if (s_GameState.bShouldSwapEffects)
	{
		m_meshEffectPair[0].effect = m_defaultEffect;
		m_meshEffectPair[1].effect = m_animColorEffect;
	}
	else
	{
		m_meshEffectPair[0].effect = m_animColorEffect;
		m_meshEffectPair[1].effect = m_defaultEffect;
	}

	//eae6320::Graphics::SubmitMeshEffectPairs(m_meshEffectPair, s_GameState.numberOfPairsToDraw);

	m_sonicGameObject->Render();
	m_planeGameObject->Render();
	m_helixGameObject->Render();

	eae6320::Graphics::SubmitCameraTransform(m_camera->GetWorldToCameraTransform(), m_camera->GetCameraToProjectedTransform_Perspective());

}

void eae6320::cFinalGame::UpdateSimulationBasedOnInput()
{
	/*if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Enter))
	{
		ReadStory();
	}*/

	/*if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Space))
	{
		
	}*/

	if (UserInput::IsKeyPressed('H'))
	{
		m_ShowImGuiUI = !m_ShowImGuiUI;
	}

	if (UserInput::IsKeyPressed('V'))
	{
		m_ShowViewportUI = !m_ShowViewportUI;
	}

	if (!m_isEndOfStory)
	{
		if (!m_BGAudio.IsPlaying())
		{
			m_BGAudio.Play();
		}
	}
	else
	{
		if (m_BGAudio.IsPlaying())
		{
			m_BGAudio.PauseAudio();
		}
	}
		
	if (m_enableScrollTrack)
	{
		m_enableScrollTrack = false;
	}

	if (m_ShowImGuiUI)
	{
		m_sonicGameObject->Move(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
		m_camera->Move(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
		return;
	}

	//Camera Movement
	float velocity = 1.25f;

	if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Left))
	{
		m_camera->Move(eae6320::Math::sVector(-velocity, 0.0f, 0.0f));
	}
	else if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Right))
	{
		m_camera->Move(eae6320::Math::sVector(velocity, 0.0f, 0.0f));
	}
	else if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Up))
	{
		m_camera->Move(eae6320::Math::sVector(0.0f, velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Down))
	{
		m_camera->Move(eae6320::Math::sVector(0.0f, -velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed('Q'))
	{
		m_camera->Move(eae6320::Math::sVector(0.0f, 0.0f, velocity * 4));
	}
	else if (UserInput::IsKeyPressed('E'))
	{
		m_camera->Move(eae6320::Math::sVector(0.0f, 0.0f, -velocity * 4));
	}
	/*else
	{
		m_camera->Move(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
	}*/


	//Game Object Movement
	//velocity = 0.75f;
	if (UserInput::IsKeyPressed('W'))
	{
		m_sonicGameObject->Move(eae6320::Math::sVector(0.0f, velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed('A'))
	{
		m_sonicGameObject->Move(eae6320::Math::sVector(-velocity, 0.0f, 0.0f));
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		m_sonicGameObject->Move(eae6320::Math::sVector(0.0f, -velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed('D'))
	{
		m_sonicGameObject->Move(eae6320::Math::sVector(velocity, 0.0f, 0.0f));
	}
	/*else
	{
		m_newGameObject->Move(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
	}*/

}

void eae6320::cFinalGame::ReadStory()
{
	if (story.canRead())
	{
		std::string dialogue = story.Read();
		if (dialogue.rfind("#", 0) == 0) 
		{ 
			//Skip lines that start with #
		}
		else
		{
			story_string.append("\n");
			story_string.append(dialogue);
		}
		
	}
	else
	{
		std::vector<std::string> choices = story.GetChoices();
		size_t choiceCount = choices.size();
		if (choiceCount > 0)
		{
			if (!m_displayChoices && !m_choiceSelected)
			{
				m_displayChoices = true;
			}

			if (!m_displayChoices && m_choiceSelected)
			{
				story_string.append("\n-----------------------------------------------------\n");
				story.SelectChoice((std::uint32_t)(m_selectedChoice));
				m_choiceSelected = false;
				ReadStory();
				m_enableScrollTrack = true;
			}
		}
		else
		{
			//End of Story
			if (!m_isEndOfStory)
			{
				m_isEndOfStory = true;
				return;
			}
		}
	}
}

void eae6320::cFinalGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{

}

void eae6320::cFinalGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	//Update Camera
	m_camera->Update(i_elapsedSecondCount_sinceLastUpdate);

	//Update RigidBody
	m_sonicGameObject->Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Run
//----

void eae6320::cFinalGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	auto result = eae6320::Results::Success;

	RegisterOnImGuiRenderUI(std::bind(&cFinalGame::RenderUI, this));

	result = InitializeStory();

	result = InitializeAudio();

	result = InitializeUI();

	// Initialize the shading data
	{
		if (!(result = InitializeShadingData()))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the shading data");
			return result;
		}
	}
	// Initialize the geometry
	{
		if (!(result = InitializeGeometry()))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the geometry data");
			return result;
		}
	}
	// Initialize the Mesh Effect Pairs
	m_meshEffectPair = new eae6320::Graphics::MeshEffectPair[MAXIMUM_NUMBER_OF_PAIRS];

	InitializePairs();

	InitializeGameObjects();

	s_GameState.numberOfPairsToDraw = s_numberOfPairsToRender;

	m_camera = new eae6320::Camera::cCamera(eae6320::Math::sVector(0.0f, 0.0f, 10.0f));

	eae6320::Logging::OutputMessage("My Game Initialized");

	//Size Information:
	size_t sizeOfMeshClass = sizeof(eae6320::Graphics::cMesh);
	size_t sizeOfEffectClass = sizeof(eae6320::Graphics::cEffect);
	eae6320::Logging::OutputMessage("Size of Mesh in  %s  using sizeof operator : %zu bytes", GetPlatformName(), sizeOfMeshClass);
	eae6320::Logging::OutputMessage("Size of Effect in  %s  using sizeof operator : %zu bytes", GetPlatformName(), sizeOfEffectClass);

	return result;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	auto result = eae6320::Results::Success;

	if (m_planeMesh)
	{
		m_planeMesh->DecrementReferenceCount();
		m_planeMesh = nullptr;
	}
	if (m_sonicMesh)
	{
		m_sonicMesh->DecrementReferenceCount();
		m_sonicMesh = nullptr;
	}
	if (m_sphereLargeMesh)
	{
		m_sphereLargeMesh->DecrementReferenceCount();
		m_sphereLargeMesh = nullptr;
	}
	if (m_helixMesh)
	{
		m_helixMesh->DecrementReferenceCount();
		m_helixMesh = nullptr;
	}

	if (m_animColorEffect)
	{
		m_animColorEffect->DecrementReferenceCount();
		m_animColorEffect = nullptr;
	}
	if (m_defaultEffect)
	{
		m_defaultEffect->DecrementReferenceCount();
		m_defaultEffect = nullptr;
	}

	size_t numberOfPairsToRender = s_numberOfPairsToRender;

	for (size_t index = 0; index < numberOfPairsToRender; index++)
	{
		if (m_meshEffectPair[index].mesh != nullptr)
		{
			m_meshEffectPair[index].mesh->DecrementReferenceCount();
			m_meshEffectPair[index].mesh = nullptr;
		}
		if (m_meshEffectPair[index].effect != nullptr)
		{
			m_meshEffectPair[index].effect->DecrementReferenceCount();
			m_meshEffectPair[index].effect = nullptr;
		}
	}

	if (m_meshEffectPair)
	{
		delete[] m_meshEffectPair;
		m_meshEffectPair = nullptr;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}

	if (m_sonicGameObject)
	{
		delete m_sonicGameObject;
		m_sonicGameObject = nullptr;
	}

	if (m_planeGameObject)
	{
		delete m_planeGameObject;
		m_planeGameObject = nullptr;
	}

	if (m_helixGameObject)
	{
		delete m_helixGameObject;
		m_helixGameObject = nullptr;
	}

	eae6320::Logging::OutputMessage("My Game CleanUp");
	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::InitializeUI()
{
	auto result = eae6320::Results::Success;

	ImGuiIO& io = ImGui::GetIO();
	m_latoFont = io.Fonts->AddFontFromFileTTF("data/fonts/Lato-Light.ttf", 24.0f);

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializeStory()
{
	auto result = eae6320::Results::Success;

	story.FromJSONFile("data/stories/vikram_vedha.storyasset");

	/*
	while (story.canRead())
	{
		while (story.canRead())
		{
			std::string dialogue = story.Read();
			if (!dialogue.empty())
			{
				story_string.append("\n");
				story_string.append(dialogue);
				eae6320::Logging::OutputMessage(dialogue.c_str());
				// 				std::cout << dialogue << std::endl;
			}
		}

		std::vector<std::string> choices = story.GetChoices();
		size_t choiceCount = choices.size();
		if (choiceCount > 0)
		{
			size_t choiceIndex = 0;
			for (const std::string& choiceText : choices)
			{
				const std::string message = "Choice Index: " + std::to_string(choiceIndex) + std::string("\t") + choiceText;
				// 				std::cout << message << std::endl;
				story_string.append("\n");
				story_string.append(message);
				eae6320::Logging::OutputMessage(message.c_str());
				choiceIndex++;
			}

			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<size_t> distrib(0, (choiceCount - 1));

			auto selectChoiceIndex = distrib(gen);
			const std::string message = "Selecting Choice Index: " + std::to_string(selectChoiceIndex);
			// 			std::cout << message << std::endl;
			story_string.append("\n");
			story_string.append(message);
			eae6320::Logging::OutputMessage(message.c_str());
			story.SelectChoice((std::uint32_t)(selectChoiceIndex));
		}
		else
		{
			//End of Story
			break;
		}
	}
	*/

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializeAudio()
{
	auto result = eae6320::Results::Success;
	
	m_BGAudio.CreateAudioData("data/audios/bg.mp3", "bg_audio", 150, true);
	m_SelectAudio.CreateAudioData("data/audios/select_choice.wav", "select_choice", 1000, false);

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializeGeometry()
{
	auto result = eae6320::Results::Success;

	// Initialize Meshes
	// Direct3D is left-handed	
	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/plane.bmasset", m_planeMesh);

	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/sonic.bmasset", m_sonicMesh);

	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/sphere.bmasset", m_sphereLargeMesh);

	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/helix.bmasset", m_helixMesh);

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializeShadingData()
{
	auto result = eae6320::Results::Success;

	// Initialize Effects

	result = eae6320::Graphics::cEffect::CreateEffect(m_animColorEffect, "data/Shaders/Fragment/testsample.bsasset");

	result = eae6320::Graphics::cEffect::CreateEffect(m_defaultEffect);

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializePairs()
{
	auto result = eae6320::Results::Success;

	m_meshEffectPair[0].mesh = m_planeMesh;
	m_meshEffectPair[0].effect = m_animColorEffect;
	eae6320::Math::cQuaternion mesh_rotation1(eae6320::Math::ConvertDegreesToRadians(45), eae6320::Math::sVector(0.0f, 0.0f, 1.0f));
	eae6320::Math::sVector mesh_position1(1.0f, 1.0f, 0.0f);
	m_meshEffectPair[0].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(mesh_rotation1, mesh_position1);
	s_numberOfPairsToRender++;
	m_meshEffectPair[1].mesh = m_sonicMesh;
	m_meshEffectPair[1].effect = m_defaultEffect;
	eae6320::Math::cQuaternion mesh_rotation2 = eae6320::Math::cQuaternion();
	eae6320::Math::sVector mesh_position2(0.0f, 0.0f, 0.0f);
	m_meshEffectPair[1].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(mesh_rotation2, mesh_position2);
	s_numberOfPairsToRender++;

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializeGameObjects()
{
	auto result = eae6320::Results::Success;

	eae6320::Math::cQuaternion game_object_rotation(eae6320::Math::ConvertDegreesToRadians(0), eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
	eae6320::Math::sVector game_object_position(1.0f, -1.0f, 3.5f);
	m_sonicGameObject = new eae6320::GameFramework::cGameObject(game_object_position, game_object_rotation);
	m_sonicGameObject->AddMeshEffectPair(m_sonicMesh, m_defaultEffect);
	m_sonicGameObject->AddMeshEffectPair(m_sphereLargeMesh, m_animColorEffect);

	m_planeGameObject = new eae6320::GameFramework::cGameObject();
	m_planeGameObject->AddMeshEffectPair(m_planeMesh, m_defaultEffect);

	m_helixGameObject = new eae6320::GameFramework::cGameObject();
	m_helixGameObject->AddMeshEffectPair(m_helixMesh, m_defaultEffect);

	return result;
}


void eae6320::cFinalGame::GetDefaultInitialResolution(uint16_t& o_width, uint16_t& o_height) const
{
	o_width = 900;
	o_height = 900;
}

void eae6320::cFinalGame::RenderUI()
{
	if (!m_ShowImGuiUI)
	{
		return;
	}

	ImGui::PushFont(m_latoFont);
	
	{
		ImGui::Begin("Vikram Vedha", NULL, window_flags);

		if (m_showStartStory)
		{
			if (ImGui::Button("Start Story"))
			{
				m_SelectAudio.PlayIndependent();
				
				m_showStartStory = false;
				ReadStory();				
			}
		}
		else
		{
			if (!m_isEndOfStory)
			{
				ReadStory();
			}
		}
		
		ImGui::TextWrapped("%s", story_string.c_str());
		if (m_enableScrollTrack)
		{
			ImGui::SetScrollHereY(1.0f);
		}
		ImGui::Text("    ");
		ImGui::End();
	}	
	{
		if (m_displayChoices)
		{
			ImGui::Begin("Choices", NULL, window_flags);

			std::vector<std::string> choices = story.GetChoices();
			size_t choiceCount = choices.size();

			for (size_t choiceIndex = 0; choiceIndex < choiceCount; choiceIndex++)
			{
				const std::string& choiceText = choices[choiceIndex];
				if (ImGui::Button(choiceText.c_str()))
				{
					m_SelectAudio.PlayIndependent();
					m_enableScrollTrack = true;
					m_selectedChoice = choiceIndex;
					m_choiceSelected = true;
					m_displayChoices = false;
					ReadStory();
				}
			}

			ImGui::End();
		}
	}
	ImGui::PopFont();
}
