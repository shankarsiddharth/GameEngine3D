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
	:
	verticalFieldOfView_inDegrees(i_verticalFieldOfView_inDegrees),
	aspectRatio(i_aspectRatio),
	z_nearPlane(i_z_nearPlane),
	z_farPlane(i_z_farPlane)
{
	m_rigidBodyState.position = i_position;
	m_rigidBodyState.orientation = i_orientation;
	
	m_worldToCameraTransform = Math::cMatrix_transformation::CreateWorldToCameraTransform(i_orientation, i_position);

	float verticalFieldOfView_inRadians = Math::ConvertDegreesToRadians(verticalFieldOfView_inDegrees);
	m_cameraToProjectedTransform_perspective = Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(
		verticalFieldOfView_inRadians, aspectRatio, z_nearPlane, z_farPlane);
}

// Initialize / Clean Up
//----------------------

eae6320::Camera::cCamera::~cCamera()
{

}

void eae6320::Camera::cCamera::Update(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_rigidBodyState.Update(i_elapsedSecondCount_sinceLastUpdate);
	UpdateTransforms(i_elapsedSecondCount_sinceLastUpdate);
}

void eae6320::Camera::cCamera::Move(const Math::sVector& i_directionVector)
{
	m_rigidBodyState.velocity = i_directionVector;
}

void eae6320::Camera::cCamera::UpdateTransforms(const float i_elapsedSecondCount_sinceLastUpdate)
{
	eae6320::Math::sVector position = m_rigidBodyState.PredictFuturePosition(i_elapsedSecondCount_sinceLastUpdate);
	eae6320::Math::cQuaternion orientation = m_rigidBodyState.PredictFutureOrientation(i_elapsedSecondCount_sinceLastUpdate);
	m_worldToCameraTransform = Math::cMatrix_transformation::CreateWorldToCameraTransform(orientation, position);

	float verticalFieldOfView_inRadians = Math::ConvertDegreesToRadians(verticalFieldOfView_inDegrees);
	m_cameraToProjectedTransform_perspective = Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(
		verticalFieldOfView_inRadians, aspectRatio, z_nearPlane, z_farPlane);
}

eae6320::Math::cMatrix_transformation eae6320::Camera::cCamera::GetWorldToCameraTransform() const
{
	return m_worldToCameraTransform;
}

eae6320::Math::cMatrix_transformation eae6320::Camera::cCamera::GetCameraToProjectedTransform_Perspective() const
{
	return m_cameraToProjectedTransform_perspective;
}

