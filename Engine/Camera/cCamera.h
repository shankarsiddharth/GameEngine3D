#pragma once

#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Math/cMatrix_transformation.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Camera
	{
		class cCamera
		{

		public:

			cCamera(const Math::sVector& i_position = Math::sVector(), 
				const Math::cQuaternion& i_orientation = Math::cQuaternion(), 
				const float i_verticalFieldOfView_inDegrees = 45, 
				const float i_aspectRatio = 1.0f,
				const float i_z_nearPlane = 0.001f, 
				const float i_z_farPlane = 1000.0f);
			~cCamera();

			void Update(const float i_elapsedSecondCount_sinceLastUpdate);

			void Move(const Math::sVector& i_directionVector);

			eae6320::Math::cMatrix_transformation GetWorldToCameraTransform() const;
			eae6320::Math::cMatrix_transformation GetCameraToProjectedTransform_Perspective() const;

		private:
			
			void UpdateTransforms(const float i_elapsedSecondCount_sinceLastUpdate);

			eae6320::Physics::sRigidBodyState m_rigidBodyState;
			eae6320::Math::cMatrix_transformation m_worldToCameraTransform;
			eae6320::Math::cMatrix_transformation m_cameraToProjectedTransform_perspective;

			const float verticalFieldOfView_inDegrees;
			const float aspectRatio;
			const float z_nearPlane;
			const float z_farPlane;
		};
	}
}