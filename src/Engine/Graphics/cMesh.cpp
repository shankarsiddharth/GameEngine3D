
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

GE3D::cResult GE3D::Graphics::cMesh::CreateMesh(GE3D::Graphics::VertexFormats::sVertex_mesh* i_vertexArray, size_t i_vertexArraySize,
	uint16_t* i_indexArray, size_t i_indexArraySize,
	GE3D::Graphics::cMesh*& o_mesh)
{

	auto result = Results::Success;

	GE3D_ASSERT(o_mesh == nullptr);

	cMesh* newMesh = nullptr;
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				GE3D_ASSERT(newMesh != nullptr);
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
			GE3D_ASSERTF(false, "Couldn't allocate memory for a mesh");
			Logging::OutputError("Failed to allocate memory for a mesh");
			return result;
		}
	}
	// Initialize the platform-specific mesh
	if (!(result = newMesh->Initialize(i_vertexArray, i_vertexArraySize, i_indexArray, i_indexArraySize)))
	{
		GE3D_ASSERTF(false, "Initialization of new mesh failed");
		return result;
	}

	return result;
}


GE3D::cResult GE3D::Graphics::cMesh::CreateMesh(const std::string& i_path, cMesh*& o_mesh)
{
	auto result = Results::Success;

	GE3D_ASSERT(o_mesh == nullptr);

	cMesh* newMesh = nullptr;
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				GE3D_ASSERT(newMesh != nullptr);
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
			GE3D_ASSERTF(false, "Couldn't allocate memory for a mesh");
			Logging::OutputError("Failed to allocate memory for a mesh");
			return result;
		}
	}

	// Read the file
	std::string errorMessage;
	GE3D::Platform::sDataFromFile dataFromFile;
	result = GE3D::Platform::LoadBinaryFile(i_path.c_str(), dataFromFile, &errorMessage);

	if (result == Results::Success)
	{
		size_t vertexArraySize = 0;
		size_t indexArraySize = 0;

		auto currentOffset = reinterpret_cast<uintptr_t>(dataFromFile.data);
		const auto finalOffset = currentOffset + dataFromFile.size;

		uint16_t vertexCount;
		memcpy(&vertexCount, reinterpret_cast<void*>(currentOffset), sizeof(uint16_t));
		vertexArraySize = static_cast<size_t>(vertexCount);

		currentOffset += sizeof(vertexCount);
		uint16_t indexCount;
		memcpy(&indexCount, reinterpret_cast<void*>(currentOffset), sizeof(uint16_t));
		indexArraySize = static_cast<size_t>(indexCount);

		currentOffset += sizeof(indexCount);
		auto* vertexArray = reinterpret_cast<GE3D::Graphics::VertexFormats::sVertex_mesh*>(currentOffset);

		size_t vertexDataOffset = (static_cast<size_t>(vertexCount) * sizeof(GE3D::Graphics::VertexFormats::sVertex_mesh));
		currentOffset += vertexDataOffset;
		auto* indexArray = reinterpret_cast<uint16_t*>(currentOffset);

		// Initialize the platform-specific mesh
		if (!(result = newMesh->Initialize(vertexArray, vertexArraySize, indexArray, indexArraySize)))
		{
			GE3D_ASSERTF(false, "Initialization of new mesh failed");
			return result;
		}
	}
	else
	{
		Logging::OutputError("Failed to load mesh %s\n Error: %s", i_path.c_str(), errorMessage.c_str());
		result = Results::Failure;
	}

	return result;
}

GE3D::Graphics::cMesh::~cMesh()
{
	GE3D_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	GE3D_ASSERT(result);
}