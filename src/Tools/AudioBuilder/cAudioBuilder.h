/*
	This class builds meshs
*/

#ifndef GE3D_CAUDIOBUILDER_H
#define GE3D_CAUDIOBUILDER_H

// Includes
//=========

#include <Tools/AssetBuildLibrary/iBuilder.h>

#include <Engine/Graphics/Configuration.h>
#include <Engine/Platform/Platform.h>

// Class Declaration
//==================

namespace GE3D
{
	namespace Assets
	{
		class cAudioBuilder final : public iBuilder
		{
			// Inherited Implementation
			//=========================

		private:

			// Build
			//------

			cResult Build(const std::vector<std::string>& i_arguments) final;
		};
	}
}

#endif	// GE3D_CAUDIOBUILDER_H
