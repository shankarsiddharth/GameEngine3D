// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/MeshEffectPair.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>
#include <Engine/StoryNarrator/Includes.h>
#include <random>

namespace
{
	struct GameState
	{
		size_t numberOfPairsToDraw = 0;
		bool bShouldSwapEffects = false;
	};

	GameState s_GameState;
}

size_t eae6320::cMyGame::s_numberOfPairsToRender = 0;

const char* eae6320::cMyGame::GetPlatformName() const
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

void eae6320::cMyGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime,
	const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
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

	m_sphereGameObject->Render();
	m_planeGameObject->Render();
	m_helixGameObject->Render();

	eae6320::Graphics::SubmitCameraTransform(m_camera->GetWorldToCameraTransform(), m_camera->GetCameraToProjectedTransform_Perspective());
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	// Is the user pressing the Q key?
	if (UserInput::IsKeyPressed('R'))
	{
		//eae6320::Logging::OutputMessage("Q Input Key Pressed");
		s_GameState.numberOfPairsToDraw = s_numberOfPairsToRender - 1;
	}
	else
	{
		s_GameState.numberOfPairsToDraw = s_numberOfPairsToRender;
	}

	if (UserInput::IsKeyPressed('F'))
	{
		s_GameState.bShouldSwapEffects = true;
	}
	else
	{
		s_GameState.bShouldSwapEffects = false;
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
		m_sphereGameObject->Move(eae6320::Math::sVector(0.0f, velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed('A'))
	{
		m_sphereGameObject->Move(eae6320::Math::sVector(-velocity, 0.0f, 0.0f));
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		m_sphereGameObject->Move(eae6320::Math::sVector(0.0f, -velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed('D'))
	{
		m_sphereGameObject->Move(eae6320::Math::sVector(velocity, 0.0f, 0.0f));
	}
	/*else
	{
		m_newGameObject->Move(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
	}*/

	if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Space))
	{
		m_sphereGameObject->SetActiveMeshEffectPairIndex(1);
	}
	else
	{
		m_sphereGameObject->SetActiveMeshEffectPairIndex(0);
	}

}

void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{

}


void eae6320::cMyGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	//Update Camera
	m_camera->Update(i_elapsedSecondCount_sinceLastUpdate);

	//Update RigidBody
	m_sphereGameObject->Update(i_elapsedSecondCount_sinceLastUpdate);
}

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Escape))
	{
		// Exit the application
		const auto result = Exit(EXIT_SUCCESS);
		EAE6320_ASSERT(result);
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	auto result = eae6320::Results::Success;

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

	InitializeStory();

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

eae6320::cResult eae6320::cMyGame::CleanUp()
{

	auto result = eae6320::Results::Success;

	if (m_planeMesh)
	{
		m_planeMesh->DecrementReferenceCount();
		m_planeMesh = nullptr;
	}
	if (m_sphereMesh)
	{
		m_sphereMesh->DecrementReferenceCount();
		m_sphereMesh = nullptr;
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

	if (m_sphereGameObject)
	{
		delete m_sphereGameObject;
		m_sphereGameObject = nullptr;
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

eae6320::cResult eae6320::cMyGame::InitializeGeometry()
{
	auto result = eae6320::Results::Success;

	// Initialize Meshes
	// Direct3D is left-handed	
	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/plane.bmasset", m_planeMesh);

	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/sonic.bmasset", m_sphereMesh);

	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/sphere.bmasset", m_sphereLargeMesh);

	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/helix.bmasset", m_helixMesh);

	return result;
}

eae6320::cResult eae6320::cMyGame::InitializeShadingData()
{
	auto result = eae6320::Results::Success;

	// Initialize Effects

	result = eae6320::Graphics::cEffect::CreateEffect(m_animColorEffect, "data/Shaders/Fragment/testsample.bsasset");

	result = eae6320::Graphics::cEffect::CreateEffect(m_defaultEffect);

	return result;
}

eae6320::cResult eae6320::cMyGame::InitializePairs()
{
	auto result = eae6320::Results::Success;

	m_meshEffectPair[0].mesh = m_planeMesh;
	m_meshEffectPair[0].effect = m_animColorEffect;
	eae6320::Math::cQuaternion mesh_rotation1(eae6320::Math::ConvertDegreesToRadians(45), eae6320::Math::sVector(0.0f, 0.0f, 1.0f));
	eae6320::Math::sVector mesh_position1(1.0f, 1.0f, 0.0f);
	m_meshEffectPair[0].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(mesh_rotation1, mesh_position1);
	s_numberOfPairsToRender++;
	m_meshEffectPair[1].mesh = m_sphereMesh;
	m_meshEffectPair[1].effect = m_defaultEffect;
	eae6320::Math::cQuaternion mesh_rotation2 = eae6320::Math::cQuaternion();
	eae6320::Math::sVector mesh_position2(0.0f, 0.0f, 0.0f);
	m_meshEffectPair[1].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(mesh_rotation2, mesh_position2);
	s_numberOfPairsToRender++;

	return result;
}

eae6320::cResult eae6320::cMyGame::InitializeGameObjects()
{
	auto result = eae6320::Results::Success;

	eae6320::Math::cQuaternion game_object_rotation(eae6320::Math::ConvertDegreesToRadians(0), eae6320::Math::sVector(0.0f, 1.0f, 0.0f));
	eae6320::Math::sVector game_object_position(1.0f, -1.0f, 3.5f);
	m_sphereGameObject = new eae6320::GameFramework::cGameObject(game_object_position, game_object_rotation);
	m_sphereGameObject->AddMeshEffectPair(m_sphereMesh, m_defaultEffect);
	m_sphereGameObject->AddMeshEffectPair(m_sphereLargeMesh, m_animColorEffect);

	m_planeGameObject = new eae6320::GameFramework::cGameObject();
	m_planeGameObject->AddMeshEffectPair(m_planeMesh, m_defaultEffect);

	m_helixGameObject = new eae6320::GameFramework::cGameObject();
	m_helixGameObject->AddMeshEffectPair(m_helixMesh, m_defaultEffect);

	return result;
}

eae6320::cResult eae6320::cMyGame::InitializeStory()
{
	auto result = eae6320::Results::Success;

	Narrator::Runtime::Story story;
	story.FromJSONFile("data/stories/test_story.storyasset");

	while (story.canRead())
	{
		while (story.canRead())
		{
			std::string dialogue = story.Read();
			if (!dialogue.empty())
			{
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
				eae6320::Logging::OutputMessage(message.c_str());
				choiceIndex++;
			}

			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<size_t> distrib(0, (choiceCount - 1));

			auto selectChoiceIndex = distrib(gen);
			const std::string message = "Selecting Choice Index: " + std::to_string(selectChoiceIndex);
// 			std::cout << message << std::endl;
			eae6320::Logging::OutputMessage(message.c_str());
			story.SelectChoice((std::uint32_t)(selectChoiceIndex));
		}
		else
		{
			//End of Story
			break;
		}
	}

	return result;
}

void eae6320::cMyGame::GetDefaultInitialResolution(uint16_t& o_width, uint16_t& o_height) const
{
	o_width = 900;
	o_height = 900;
}

