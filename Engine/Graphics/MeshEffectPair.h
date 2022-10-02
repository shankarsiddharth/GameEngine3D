#pragma once


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
			eae6320::Math::cMatrix_transformation localToWorld_transform;
		};
	}
}