// Includes
//=========

#include "sRigidBodyState.h"

#include <Engine/Math/cMatrix_transformation.h>

// Interface
//==========

void GE3D::Physics::sRigidBodyState::Update( const float i_secondCountToIntegrate )
{
	// Update position
	{
		position += velocity * i_secondCountToIntegrate;
	}
	// Update velocity
	{
		velocity += acceleration * i_secondCountToIntegrate;
	}
	// Update orientation
	{
		const auto rotation = Math::cQuaternion( angularSpeed * i_secondCountToIntegrate, angularVelocity_axis_local );
		orientation = orientation * rotation;
		orientation.Normalize();
	}
}

GE3D::Math::sVector GE3D::Physics::sRigidBodyState::PredictFuturePosition( const float i_secondCountToExtrapolate ) const
{
	return position + ( velocity * i_secondCountToExtrapolate );
}

GE3D::Math::cQuaternion GE3D::Physics::sRigidBodyState::PredictFutureOrientation( const float i_secondCountToExtrapolate ) const
{
	const auto rotation = Math::cQuaternion( angularSpeed * i_secondCountToExtrapolate, angularVelocity_axis_local );
	return Math::cQuaternion( orientation * rotation ).GetNormalized();
}

GE3D::Math::cMatrix_transformation GE3D::Physics::sRigidBodyState::PredictFutureTransform( const float i_secondCountToExtrapolate ) const
{
	return Math::cMatrix_transformation( PredictFutureOrientation( i_secondCountToExtrapolate ), PredictFuturePosition( i_secondCountToExtrapolate ) );
}
