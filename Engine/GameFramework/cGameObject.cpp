// Includes
//=========

#include "cGameObject.h"
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/Graphics.h>

// Implementation
//===============

eae6320::GameFramework::cGameObject::cGameObject()
{
	m_meshEffectPair = new eae6320::Graphics::MeshEffectPair[MAXIMUM_NUMBER_OF_PAIRS];
}

eae6320::GameFramework::cGameObject::cGameObject(const eae6320::Math::sVector& i_position, const eae6320::Math::cQuaternion& i_orientation)
	:cGameObject()
{
	m_rigidBodyState.position = i_position;
	m_rigidBodyState.orientation = i_orientation;
}

eae6320::GameFramework::cGameObject::~cGameObject()
{
	for (size_t index = 0; index < m_totalPairSize; index++)
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
}

void eae6320::GameFramework::cGameObject::Render()
{
	//If this fails then there are no meshes added to the game object
	EAE6320_ASSERT(m_activeMeshEffectPairIndex < m_totalPairSize);

	eae6320::Graphics::MeshEffectPair* activePair = &m_meshEffectPair[m_activeMeshEffectPairIndex];
	eae6320::Graphics::SubmitMeshEffectPairs(activePair, 1);
}

void eae6320::GameFramework::cGameObject::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
	UpdateTransforms(i_elapsedSecondCount_sinceLastUpdate);
	m_rigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::GameFramework::cGameObject::Move(const Math::sVector& i_directionVector)
{
	m_rigidBodyState.velocity = i_directionVector;
}

void eae6320::GameFramework::cGameObject::AddMeshEffectPair(eae6320::Graphics::cMesh* i_mesh, eae6320::Graphics::cEffect* i_effect)
{
	EAE6320_ASSERT(m_totalPairSize <= MAXIMUM_NUMBER_OF_PAIRS);

	const eae6320::Math::sVector& position = m_rigidBodyState.position;
	const eae6320::Math::cQuaternion& orientation = m_rigidBodyState.orientation;

	m_meshEffectPair[m_totalPairSize].mesh = i_mesh;
	m_meshEffectPair[m_totalPairSize].effect = i_effect;
	m_meshEffectPair[m_totalPairSize].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(orientation, position);
	m_totalPairSize++;
}

void eae6320::GameFramework::cGameObject::AddMeshEffectPair(const eae6320::Math::sVector& i_position, const eae6320::Math::cQuaternion& i_orientation, eae6320::Graphics::cMesh* i_mesh, eae6320::Graphics::cEffect* i_effect)
{
	EAE6320_ASSERT(m_totalPairSize <= MAXIMUM_NUMBER_OF_PAIRS);

	m_meshEffectPair[m_totalPairSize].mesh = i_mesh;
	m_meshEffectPair[m_totalPairSize].effect = i_effect;
	m_meshEffectPair[m_totalPairSize].drawCallData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation(i_orientation, i_position);
	m_totalPairSize++;
}

eae6320::Graphics::MeshEffectPair* eae6320::GameFramework::cGameObject::GetMeshEffectPairAtIndex(const size_t i_index) const
{
	EAE6320_ASSERT(i_index < m_totalPairSize);

	return &(m_meshEffectPair[i_index]);
}

eae6320::Graphics::MeshEffectPair* eae6320::GameFramework::cGameObject::GetAllMeshEffectPairs() const
{
	return nullptr;
}

size_t eae6320::GameFramework::cGameObject::GetActiveMeshEffectPairIndex() const
{
	return m_activeMeshEffectPairIndex;
}

void eae6320::GameFramework::cGameObject::SetActiveMeshEffectPairIndex(size_t i_index)
{
	EAE6320_ASSERT(i_index < m_totalPairSize);

	m_activeMeshEffectPairIndex = i_index;
}

void eae6320::GameFramework::cGameObject::UpdateTransforms(const float i_elapsedSecondCount_sinceLastUpdate)
{
	eae6320::Math::cMatrix_transformation rigidBodyTransform = m_rigidBodyState.PredictFutureTransform(i_elapsedSecondCount_sinceLastUpdate);
	m_meshEffectPair[m_activeMeshEffectPairIndex].drawCallData.g_transform_localToWorld = rigidBodyTransform;
}

inline size_t eae6320::GameFramework::cGameObject::GetTotalMeshEffectPairs() const { return m_totalPairSize; }
