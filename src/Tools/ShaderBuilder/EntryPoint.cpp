/*
	The main() function is where the program starts execution
*/

// Includes
//=========

#include "cShaderBuilder.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	return GE3D::Assets::Build<GE3D::Assets::cShaderBuilder>( i_arguments, i_argumentCount );
}
