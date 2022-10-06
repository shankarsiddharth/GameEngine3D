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
		m_meshEffectPair[0].effect = m_secondEffect;
		m_meshEffectPair[1].effect = m_newEffect;
	}
	else
	{
		m_meshEffectPair[0].effect = m_newEffect;
		m_meshEffectPair[1].effect = m_secondEffect;
	}
	
	//eae6320::Graphics::SubmitMeshEffectPairs(m_meshEffectPair, s_GameState.numberOfPairsToDraw);

	m_newGameObject->Render();

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
		m_newGameObject->Move(eae6320::Math::sVector(0.0f, velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed('A'))
	{
		m_newGameObject->Move(eae6320::Math::sVector(-velocity, 0.0f, 0.0f));
	}
	else if (UserInput::IsKeyPressed('S'))
	{
		m_newGameObject->Move(eae6320::Math::sVector(0.0f, -velocity, 0.0f));
	}
	else if (UserInput::IsKeyPressed('D'))
	{
		m_newGameObject->Move(eae6320::Math::sVector(velocity, 0.0f, 0.0f));
	}
	/*else
	{
		m_newGameObject->Move(eae6320::Math::sVector(0.0f, 0.0f, 0.0f));
	}*/

	if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Space))
	{
		m_newGameObject->SetActiveMeshEffectPairIndex(1);
	}
	else
	{
		m_newGameObject->SetActiveMeshEffectPairIndex(0);
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
	m_newGameObject->Update(i_elapsedSecondCount_sinceLastUpdate);
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
	
	if (m_newMesh)
	{
		m_newMesh->DecrementReferenceCount();
		m_newMesh = nullptr;
	}
	if (m_secondMesh)
	{
		m_secondMesh->DecrementReferenceCount();
		m_secondMesh = nullptr;
	}

	if (m_newEffect)
	{
		m_newEffect->DecrementReferenceCount();
		m_newEffect = nullptr;
	}
	if (m_secondEffect)
	{
		m_secondEffect->DecrementReferenceCount();
		m_secondEffect = nullptr;
	}

	size_t numberOfPairsToRender = s_numberOfPairsToRender;

	for (size_t index = 0; index < numberOfPairsToRender; index++)
	{
		if (m_meshEffectPair[index].mesh != nullptr)
		{
			m_meshEffectPair[index].mesh->DecrementReferenceCount();
			m_meshEffectPair[index].mesh = nullptr;
		}
		if(m_meshEffectPair[index].effect != nullptr)
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

	if (m_newGameObject)
	{
		delete m_newGameObject;
		m_newGameObject = nullptr;
	}

	eae6320::Logging::OutputMessage("My Game CleanUp");
	return Results::Success;
}

eae6320::cResult eae6320::cMyGame::InitializeGeometry()
{
	auto result = eae6320::Results::Success;

	// Initialize Meshes
	eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[] =
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

	result = eae6320::Graphics::cMesh::CreateMesh(vertexData, 4, indexData, 6, m_newMesh);

	eae6320::Graphics::VertexFormats::sVertex_mesh newVertexData[] =
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

	uint16_t newIndexData3[] =
	{
		// Direct3D is left-handed
		0, 3, 1
	};

	result = eae6320::Graphics::cMesh::CreateMesh(newVertexData, 3, newIndexData, 3, m_secondMesh);

	result = eae6320::Graphics::cMesh::CreateMesh(vertexData, 4, newIndexData3, 3, m_thirdMesh);

	result = eae6320::Graphics::cMesh::CreateMesh("data/Meshes/thirdMesh.json", m_testMesh);
	
	return result;
}

eae6320::cResult eae6320::cMyGame::InitializeShadingData()
{
	auto result = eae6320::Results::Success;

	// Initialize Effects

	result = eae6320::Graphics::cEffect::CreateEffect(m_newEffect, "data/Shaders/Fragment/testsample.shader");

	result = eae6320::Graphics::cEffect::CreateEffect(m_secondEffect);

	return result;
}

eae6320::cResult eae6320::cMyGame::InitializePairs()
{
	auto result = eae6320::Results::Success;

	m_meshEffectPair[0].mesh = m_newMesh;
	m_meshEffectPair[0].effect = m_newEffect;
	eae6320::Math::cQuaternion mesh_rotation1(eae6320::Math::ConvertDegreesToRadians(45), eae6320::Math::sVector(0.0f, 0.0f, 1.0f));
	eae6320::Math::sVector mesh_position1(1.0f, 1.0f, 0.0f);
	m_meshEffectPair[0].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(mesh_rotation1, mesh_position1);
	s_numberOfPairsToRender++;
	m_meshEffectPair[1].mesh = m_secondMesh;
	m_meshEffectPair[1].effect = m_secondEffect;
	eae6320::Math::cQuaternion mesh_rotation2 = eae6320::Math::cQuaternion();
	eae6320::Math::sVector mesh_position2(0.0f, 0.0f, 0.0f);
	m_meshEffectPair[1].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(mesh_rotation2, mesh_position2);
	s_numberOfPairsToRender++;

	return result;
}

eae6320::cResult eae6320::cMyGame::InitializeGameObjects()
{
	auto result = eae6320::Results::Success;

	//eae6320::Math::cQuaternion mesh_rotation1(eae6320::Math::ConvertDegreesToRadians(-180), eae6320::Math::sVector(0.0f, 0.0f, 1.0f));
	//eae6320::Math::sVector mesh_position1(1.0f, 1.0f, 0.0f);
	m_newGameObject = new eae6320::GameFramework::cGameObject();
	//m_newGameObject = new eae6320::GameFramework::cGameObject(mesh_position1, mesh_rotation1);
	//m_newGameObject->AddMeshEffectPair(m_newMesh, m_newEffect);
	m_newGameObject->AddMeshEffectPair(m_newMesh, m_secondEffect);
	m_newGameObject->AddMeshEffectPair(m_testMesh, m_newEffect);

	return result;
}

