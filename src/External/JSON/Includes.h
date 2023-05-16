/*
	This file can be included by a project that wants to use Niels Lohmann's JSON library

	Using this intermediate file to store the version number
	means that no other source files will have to change
	if you ever change to a new version.
*/

#ifndef GE3D_JSONINCLUDES_H
#define GE3D_JSONINCLUDES_H

// Includes
//=========

#include <Engine/Asserts/Asserts.h>

#define JSON_ASSERT( i_assertion ) GE3D_ASSERT( i_assertion )

#include "3.9.1/json.hpp"

#ifdef JSON_ASSERT
	#undef JSON_ASSERT
#endif

#endif	// GE3D_JSONINCLUDES_H
