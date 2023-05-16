#pragma once

#include "ConstantBufferFormats.h"

// Forward Declaration
//==================

namespace GE3D
{
	namespace Graphics
	{
		class cMesh;
		class cEffect;
	}
}



// Format Definitions
//===================

namespace GE3D
{
	namespace Graphics
	{
		struct MeshEffectPair
		{			
			GE3D::Graphics::cMesh* mesh = nullptr;
			GE3D::Graphics::cEffect* effect = nullptr;
			GE3D::Graphics::ConstantBufferFormats::sDrawCall drawCallData;
		};
	}
}