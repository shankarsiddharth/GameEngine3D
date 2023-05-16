#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Graphics/MeshEffectPair.h>

// Forward Declaration
//==================

namespace GE3D
{
	namespace Graphics
	{
		class cMesh;
		class cEffect;
	}
}

// Class Declaration
//==================

namespace GE3D
{
	namespace GameFramework
	{
		class cGameObject
		{

		public:

			cGameObject();
			cGameObject(const GE3D::Math::sVector& i_position, const GE3D::Math::cQuaternion& i_orientation);
			~cGameObject();

			static const size_t MAXIMUM_NUMBER_OF_PAIRS = 10;

			void Render();

			void Update(const float i_elapsedSecondCount_sinceLastUpdate);

			void Move(const Math::sVector& i_directionVector);

			void AddMeshEffectPair(GE3D::Graphics::cMesh* i_mesh, GE3D::Graphics::cEffect* i_effect);
			
			size_t GetActiveMeshEffectPairIndex() const;
			void SetActiveMeshEffectPairIndex(const size_t i_index);
		
		private:
			
			void UpdateTransforms(const float i_elapsedSecondCount_sinceLastUpdate);

			size_t GetTotalMeshEffectPairs() const;

			/*GE3D::Graphics::MeshEffectPair* GetMeshEffectPairAtIndex(const size_t i_index) const;

			GE3D::Graphics::MeshEffectPair* GetAllMeshEffectPairs() const;			
			
			void AddMeshEffectPair(const GE3D::Math::sVector& i_position, const GE3D::Math::cQuaternion& i_orientation, GE3D::Graphics::cMesh* i_mesh, GE3D::Graphics::cEffect* i_effect);
			*/

			GE3D::Physics::sRigidBodyState m_rigidBodyState;
			GE3D::Graphics::MeshEffectPair* m_meshEffectPair = nullptr;

			size_t m_activeMeshEffectPairIndex = 0;

			size_t m_totalPairSize = 0;

		};
	}
}