#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Graphics/MeshEffectPair.h>

// Forward Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh;
		class cEffect;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace GameFramework
	{
		class cGameObject
		{

		public:

			cGameObject();
			cGameObject(const eae6320::Math::sVector& i_position, const eae6320::Math::cQuaternion& i_orientation);
			~cGameObject();

			static const size_t MAXIMUM_NUMBER_OF_PAIRS = 10;

			void Render();

			void Update(const float i_elapsedSecondCount_sinceLastUpdate);

			void Move(const Math::sVector& i_directionVector);

			size_t GetTotalMeshEffectPairs() const;

			void AddMeshEffectPair(const eae6320::Math::sVector& i_position, const eae6320::Math::cQuaternion& i_orientation, eae6320::Graphics::cMesh* i_mesh, eae6320::Graphics::cEffect* i_effect);
			void AddMeshEffectPair(eae6320::Graphics::cMesh* i_mesh, eae6320::Graphics::cEffect* i_effect);
			
			size_t GetActiveMeshEffectPairIndex() const;
			void SetActiveMeshEffectPairIndex(const size_t i_index);
		
		private:
			
			void UpdateTransforms(const float i_elapsedSecondCount_sinceLastUpdate);

			eae6320::Graphics::MeshEffectPair* GetMeshEffectPairAtIndex(const size_t i_index) const;

			eae6320::Graphics::MeshEffectPair* GetAllMeshEffectPairs() const;


			eae6320::Physics::sRigidBodyState m_rigidBodyState;
			eae6320::Graphics::MeshEffectPair* m_meshEffectPair = nullptr;

			size_t m_activeMeshEffectPairIndex = 0;

			size_t m_totalPairSize = 0;

		};
	}
}