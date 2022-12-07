// Includes
//=========

#include "cFontBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Asserts/Asserts.h>

// Build
//------

eae6320::cResult eae6320::Assets::cFontBuilder::Build(const std::vector<std::string>& i_arguments)
{
	eae6320::cResult result = Results::Success;


	if (!(result = Platform::CopyFileW(m_path_source, m_path_target, false, true)))
	{
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Fail to copy font file!");
	}

	return result;
}