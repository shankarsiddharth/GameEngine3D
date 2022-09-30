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
	eae6320::Graphics::SetBackgroundClearColor(0.0f, 0.95f, 1.0f);
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
	eae6320::Graphics::SubmitMeshEffectPairs(m_meshEffectPair, s_GameState.numberOfPairsToDraw);
}

void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	// Is the user pressing the Q key?
	if (UserInput::IsKeyPressed('Q'))
	{
		//eae6320::Logging::OutputMessage("Q Input Key Pressed");
		s_GameState.numberOfPairsToDraw = s_numberOfPairsToRender - 1;
	}
	else
	{
		s_GameState.numberOfPairsToDraw = s_numberOfPairsToRender;
	}

	if (UserInput::IsKeyPressed('E'))
	{
		s_GameState.bShouldSwapEffects = true;
	}
	else
	{
		s_GameState.bShouldSwapEffects = false;
	}
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

	s_GameState.numberOfPairsToDraw = s_numberOfPairsToRender;

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

	result = eae6320::Graphics::cMesh::CreateMesh(newVertexData, 3, newIndexData, 3, m_secondMesh);

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
	s_numberOfPairsToRender++;
	m_meshEffectPair[1].mesh = m_secondMesh;
	m_meshEffectPair[1].effect = m_secondEffect;
	s_numberOfPairsToRender++;

	return result;
}
