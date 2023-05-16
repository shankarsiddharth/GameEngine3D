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

GE3D::GameFramework::cGameObject::cGameObject()
{
	m_meshEffectPair = new GE3D::Graphics::MeshEffectPair[MAXIMUM_NUMBER_OF_PAIRS];
}

GE3D::GameFramework::cGameObject::cGameObject(const GE3D::Math::sVector& i_position, const GE3D::Math::cQuaternion& i_orientation)
	:cGameObject()
{
	m_rigidBodyState.position = i_position;
	m_rigidBodyState.orientation = i_orientation;
}

GE3D::GameFramework::cGameObject::~cGameObject()
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

void GE3D::GameFramework::cGameObject::Render()
{
	//If this fails then there are no meshes added to the game object
	GE3D_ASSERT(m_activeMeshEffectPairIndex < m_totalPairSize);

	GE3D::Graphics::MeshEffectPair* activePair = &m_meshEffectPair[m_activeMeshEffectPairIndex];
	GE3D::Graphics::SubmitMeshEffectPairs(activePair, 1);
}

void GE3D::GameFramework::cGameObject::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
	UpdateTransforms(i_elapsedSecondCount_sinceLastUpdate);
	m_rigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
}

void GE3D::GameFramework::cGameObject::Move(const Math::sVector& i_directionVector)
{
	m_rigidBodyState.velocity = i_directionVector;
}

void GE3D::GameFramework::cGameObject::AddMeshEffectPair(GE3D::Graphics::cMesh* i_mesh, GE3D::Graphics::cEffect* i_effect)
{
	GE3D_ASSERT(m_totalPairSize <= MAXIMUM_NUMBER_OF_PAIRS);

	const GE3D::Math::sVector& position = m_rigidBodyState.position;
	const GE3D::Math::cQuaternion& orientation = m_rigidBodyState.orientation;

	m_meshEffectPair[m_totalPairSize].mesh = i_mesh;
	m_meshEffectPair[m_totalPairSize].effect = i_effect;
	m_meshEffectPair[m_totalPairSize].drawCallData.g_transform_localToWorld = GE3D::Math::cMatrix_transformation(orientation, position);
	m_totalPairSize++;
}

//void GE3D::GameFramework::cGameObject::AddMeshEffectPair(const GE3D::Math::sVector& i_position, const GE3D::Math::cQuaternion& i_orientation, GE3D::Graphics::cMesh* i_mesh, GE3D::Graphics::cEffect* i_effect)
//{
//	GE3D_ASSERT(m_totalPairSize <= MAXIMUM_NUMBER_OF_PAIRS);
//
//	m_meshEffectPair[m_totalPairSize].mesh = i_mesh;
//	m_meshEffectPair[m_totalPairSize].effect = i_effect;
//	m_meshEffectPair[m_totalPairSize].drawCallData.g_transform_localToWorld = GE3D::Math::cMatrix_transformation(i_orientation, i_position);
//	m_totalPairSize++;
//}

//GE3D::Graphics::MeshEffectPair* GE3D::GameFramework::cGameObject::GetMeshEffectPairAtIndex(const size_t i_index) const
//{
//	GE3D_ASSERT(i_index < m_totalPairSize);
//
//	return &(m_meshEffectPair[i_index]);
//}
//
//GE3D::Graphics::MeshEffectPair* GE3D::GameFramework::cGameObject::GetAllMeshEffectPairs() const
//{
//	return nullptr;
//}

size_t GE3D::GameFramework::cGameObject::GetActiveMeshEffectPairIndex() const
{
	return m_activeMeshEffectPairIndex;
}

void GE3D::GameFramework::cGameObject::SetActiveMeshEffectPairIndex(size_t i_index)
{
	GE3D_ASSERT(i_index < m_totalPairSize);

	m_activeMeshEffectPairIndex = i_index;
}

void GE3D::GameFramework::cGameObject::UpdateTransforms(const float i_elapsedSecondCount_sinceLastUpdate)
{
	GE3D::Math::cMatrix_transformation rigidBodyTransform = m_rigidBodyState.PredictFutureTransform(i_elapsedSecondCount_sinceLastUpdate);
	m_meshEffectPair[m_activeMeshEffectPairIndex].drawCallData.g_transform_localToWorld = rigidBodyTransform;
}

inline size_t GE3D::GameFramework::cGameObject::GetTotalMeshEffectPairs() const { return m_totalPairSize; }
