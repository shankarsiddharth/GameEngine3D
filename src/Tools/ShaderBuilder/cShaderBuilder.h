/*
	This class builds shaders
*/

#ifndef GE3D_CSHADERBUILDER_H
#define GE3D_CSHADERBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>

#include <Engine/Graphics/Configuration.h>
#include <Engine/Graphics/cShader.h>

// Class Declaration
//==================

namespace GE3D
{
	namespace Assets
	{
		class cShaderBuilder final : public iBuilder
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

			// Build
			//------

			cResult Build( const Graphics::eShaderType i_shaderType, const std::vector<std::string>& i_arguments );
		};
	}
}

#endif	// GE3D_CSHADERBUILDER_H
