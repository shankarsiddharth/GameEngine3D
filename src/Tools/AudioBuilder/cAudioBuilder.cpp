// Includes
//=========

#include "cAudioBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Asserts/Asserts.h>

// Build
//------

GE3D::cResult GE3D::Assets::cAudioBuilder::Build(const std::vector<std::string>& i_arguments)
{
	GE3D::cResult result = Results::Success;


	if (!(result = Platform::CopyFileW(m_path_source, m_path_target, false, true)))
	{
		GE3D::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Fail to copy audio file!");
	}

	return result;
}