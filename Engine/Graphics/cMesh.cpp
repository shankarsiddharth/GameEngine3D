
#include "cMesh.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Platform/Platform.h>
#include <External/JSON/Includes.h>

#include <cmath>
#include <limits>

// Factory Method
//-------

eae6320::cResult eae6320::Graphics::cMesh::CreateMesh(eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexArray, size_t i_vertexArraySize,
	uint16_t* i_indexArray, size_t i_indexArraySize,
	eae6320::Graphics::cMesh*& o_mesh)
{

	auto result = Results::Success;

	EAE6320_ASSERT(o_mesh == nullptr);

	cMesh* newMesh = nullptr;
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
	{
		if (result)
		{
			EAE6320_ASSERT(newMesh != nullptr);
			o_mesh = newMesh;
		}
		else
		{
			if (newMesh)
			{
				newMesh->DecrementReferenceCount();
				newMesh = nullptr;
			}
			o_mesh = nullptr;
		}
	});

	// Allocate a new Mesh
	{
		newMesh = new (std::nothrow) cMesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for a mesh");
			Logging::OutputError("Failed to allocate memory for a mesh");
			return result;
		}
	}
	// Initialize the platform-specific mesh
	if (!(result = newMesh->Initialize(i_vertexArray, i_vertexArraySize, i_indexArray, i_indexArraySize)))
	{
		EAE6320_ASSERTF(false, "Initialization of new mesh failed");
		return result;
	}

	return result;
}


eae6320::cResult eae6320::Graphics::cMesh::CreateMesh(const std::string& i_path, cMesh*& o_mesh)
{
	auto result = Results::Success;

	EAE6320_ASSERT(o_mesh == nullptr);

	cMesh* newMesh = nullptr;
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				EAE6320_ASSERT(newMesh != nullptr);
				o_mesh = newMesh;
			}
			else
			{
				if (newMesh)
				{
					newMesh->DecrementReferenceCount();
					newMesh = nullptr;
				}
				o_mesh = nullptr;
			}
		});

	// Allocate a new Mesh
	{
		newMesh = new (std::nothrow) cMesh();
		if (!newMesh)
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for a mesh");
			Logging::OutputError("Failed to allocate memory for a mesh");
			return result;
		}
	}

	size_t vertexArraySize = 0;
	size_t indexArraySize = 0;

	eae6320::Graphics::VertexFormats::sVertex_mesh* vertexArray = nullptr;
	uint16_t* indexArray = nullptr;

	// Read the file
	{
		std::string errorMessage;
		eae6320::Platform::sDataFromFile dataFromFile;
		if (eae6320::Platform::LoadBinaryFile(i_path.c_str(), dataFromFile, &errorMessage))
		{
			result = Results::Failure;

			auto currentOffset = reinterpret_cast<uintptr_t>(dataFromFile.data);
			const auto finalOffset = currentOffset + dataFromFile.size;

			uint16_t vertexArraySize;
			memcpy(&vertexArraySize, reinterpret_cast<void*>(currentOffset), sizeof(vertexArraySize));

			currentOffset += sizeof(vertexArraySize);
			uint16_t indexArraySize;
			memcpy(&indexArraySize, reinterpret_cast<void*>(currentOffset), sizeof(indexArraySize));


			currentOffset += sizeof(indexArraySize);
			const auto* vertexArray = reinterpret_cast<eae6320::Graphics::VertexFormats::sVertex_mesh*>(currentOffset);

			currentOffset += (vertexArraySize * sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh));
			const auto* indexArray = reinterpret_cast<uint16_t*>(currentOffset);	

			result = Results::Success;
		}
		else
		{
			Logging::OutputError("Failed to load mesh %s\n Error: %s", i_path.c_str(), errorMessage.c_str());
			result = Results::Failure;
		}
	}	

	if (result == Results::Success)
	{
		// Initialize the platform-specific mesh
		if (!(result = newMesh->Initialize(vertexArray, vertexArraySize, indexArray, indexArraySize)))
		{
			EAE6320_ASSERTF(false, "Initialization of new mesh failed");
			return result;
		}		
	}
	else
	{
		//Error: Failed to load mesh
	}	

	delete[] vertexArray;
	vertexArray = nullptr;
	delete[] indexArray;
	indexArray = nullptr;

	return result;
}

eae6320::Graphics::cMesh::~cMesh()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}