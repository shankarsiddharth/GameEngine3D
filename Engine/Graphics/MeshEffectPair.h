#pragma once

#include "ConstantBufferFormats.h"

// Forward Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cMesh;
		class cEffect;
	}
}



// Format Definitions
//===================

namespace eae6320
{
	namespace Graphics
	{
		struct MeshEffectPair
		{			
			eae6320::Graphics::cMesh* mesh = nullptr;
			eae6320::Graphics::cEffect* effect = nullptr;
			eae6320::Graphics::ConstantBufferFormats::sDrawCall drawCallData;
		};
	}
}