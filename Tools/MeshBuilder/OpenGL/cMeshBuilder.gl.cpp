// Includes
//=========

#include "../cMeshBuilder.h"

eae6320::cResult eae6320::Assets::cMeshBuilder::ProcessIndexArray(uint16_t* i_indexArray, size_t i_indexArraySize)
{
	auto result = Results::Success;

	for (size_t index = 0; index < i_indexArraySize; index += 3)
	{
		uint16_t cacheValue = i_indexArray[index + 2];
		i_indexArray[index + 2] = i_indexArray[index + 1];
		i_indexArray[index + 1] = cacheValue;
	}

	return result;
}