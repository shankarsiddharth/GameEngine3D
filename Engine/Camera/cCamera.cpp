// Includes
//=========

#include "cCamera.h"
#include <Engine/Math/Functions.h>

// Implementation
//===============

eae6320::Camera::cCamera::cCamera(const Math::sVector& i_position /*= Math::sVector()*/, 
	const Math::cQuaternion& i_orientation /*= Math::cQuaternion()*/, 
	const float i_verticalFieldOfView_inDegrees /*= 45*/,
	const float i_aspectRatio /*= 1.0f*/,
	const float i_z_nearPlane /*= 0.1f*/, 
	const float i_z_farPlane /*= 10.0f*/)	
{
	m_worldToCameraTransform = Math::cMatrix_transformation::CreateWorldToCameraTransform(i_orientation, i_position);
	
	float verticalFieldOfView_inRadians = Math::ConvertDegreesToRadians(i_verticalFieldOfView_inDegrees);
	m_cameraToProjectedTransform_perspective = Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(
		verticalFieldOfView_inRadians, i_aspectRatio, i_z_nearPlane, i_z_farPlane
	);
}

// Initialize / Clean Up
//----------------------

eae6320::Camera::cCamera::~cCamera()
{

}

eae6320::Math::cMatrix_transformation eae6320::Camera::cCamera::GetWorldToCameraTransform() const
{
	return m_worldToCameraTransform;
}

eae6320::Math::cMatrix_transformation eae6320::Camera::cCamera::GetCameraToProjectedTransform_Perspective() const
{
	return m_cameraToProjectedTransform_perspective;
}

