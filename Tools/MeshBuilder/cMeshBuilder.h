/*
	This class builds shaders
*/

#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>

#include <Engine/Graphics/Configuration.h>

// Class Declaration
//==================

namespace eae6320
{
	namespace Assets
	{
		class cMeshBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build( const std::vector<std::string>& i_arguments ) final;

			// Implementation
			//===============

		private:

			cResult ProcessIndexArray(uint16_t* i_indexArray, size_t i_indexArraySize);

		};
	}
}

#endif	// EAE6320_CMESHBUILDER_H
