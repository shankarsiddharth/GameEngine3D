// Includes
//=========

#include "sVector.h"

#include <cmath>
#include <Engine/Asserts/Asserts.h>

// Static Data
//============

namespace
{
	constexpr auto s_epsilon = 1.0e-9f;
}

// Interface
//==========

// Division
//---------

GE3D::Math::sVector GE3D::Math::sVector::operator /( const float i_rhs ) const
{
	GE3D_ASSERTF( std::abs( i_rhs ) > s_epsilon, "Can't divide by zero" );
	return sVector( x / i_rhs, y / i_rhs, z / i_rhs );
}

GE3D::Math::sVector& GE3D::Math::sVector::operator /=( const float i_rhs )
{
	GE3D_ASSERTF( std::abs( i_rhs ) > s_epsilon, "Can't divide by zero" );
	x /= i_rhs;
	y /= i_rhs;
	z /= i_rhs;
	return *this;
}

// Length / Normalization
//-----------------------

float GE3D::Math::sVector::GetLength() const
{
	const auto length_squared = ( x * x ) + ( y * y ) + ( z * z );
	GE3D_ASSERTF( length_squared >= 0.0f, "Can't take a square root of a negative number" );
	return std::sqrt( length_squared );
}

float GE3D::Math::sVector::Normalize()
{
	const auto length = GetLength();
	GE3D_ASSERTF( length > s_epsilon, "Can't divide by zero" );
	operator /=( length );
	return length;
}

GE3D::Math::sVector GE3D::Math::sVector::GetNormalized() const
{
	const auto length = GetLength();
	GE3D_ASSERTF( length > s_epsilon, "Can't divide by zero" );
	return sVector( x / length, y / length, z / length );
}
