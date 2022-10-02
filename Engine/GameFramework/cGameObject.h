#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Graphics/MeshEffectPair.h>

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
			~cGameObject();

			void Update(const float i_elapsedSecondCount_sinceLastUpdate);

			void Move(const Math::sVector& i_directionVector);

		private:
			
			void UpdateTransforms(const float i_elapsedSecondCount_sinceLastUpdate);

			eae6320::Physics::sRigidBodyState m_rigidBodyState;
			eae6320::Graphics::MeshEffectPair m_meshEffectPair;

		};
	}
}