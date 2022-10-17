
#include "cMesh.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Platform/Platform.h>
#include <External/JSON/Includes.h>

#include <cmath>

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

	// Read the json file
	eae6320::Platform::sDataFromFile dataFromFile;
	eae6320::Platform::LoadBinaryFile(i_path.c_str(), dataFromFile);

	const auto parsedFile = nlohmann::json::parse(static_cast<const char*>(dataFromFile.data),
		static_cast<const char*>(dataFromFile.data) + dataFromFile.size);
	
	size_t vertexArrayCount = 0;
	size_t indexArrayCount = 0;

	// TODO: Error Check max size and print log information and error
	auto* vertexArray = new eae6320::Graphics::VertexFormats::sVertex_mesh[std::numeric_limits<uint16_t>::max()];
	auto* indexArray = new uint16_t[std::numeric_limits<uint16_t>::max()];
	
	if (parsedFile.is_object())
	{
		//Parse Vertex Data
		{
			const auto vertexDataArray = parsedFile["vertex_data"];
			if (vertexDataArray.is_array())
			{
				for (const auto& vertexData : vertexDataArray)
				{
					if (vertexData.is_object())
					{
						eae6320::Graphics::VertexFormats::sVertex_mesh vertex{};

						if (vertexData.contains("position"))
						{
							const auto position = vertexData["position"];
							const auto position_x = position["x"];
							const auto position_y = position["y"];
							const auto position_z = position["z"];
							if (position_x.is_number())
							{
								vertex.x = position_x.get<float>();
							}
							if (position_x.is_number())
							{
								vertex.y = position_y.get<float>();
							}
							if (position_x.is_number())
							{
								vertex.z = position_z.get<float>();
							}							
						}
						else
						{
							//TODO: Log Parse Error
							//Should have position value
							EAE6320_ASSERT(false);
						}

						if (vertexData.contains("color"))
						{
							const auto color = vertexData["color"];
							const auto color_r = color["r"];
							const auto color_g = color["g"];
							const auto color_b = color["b"];
							const auto color_a = color["a"];
							if (color_r.is_number())
							{
								auto r = color_r.get<float>();
								vertex.r = (uint8_t)std::round(r * 255.0f);
							}
							if (color_g.is_number())
							{
								auto g = color_g.get<float>();
								vertex.g = (uint8_t)std::round(g * 255.0f);
							}
							if (color_b.is_number())
							{
								auto b = color_b.get<float>();
								vertex.b = (uint8_t)std::round(b * 255.0f);
							}
							if (color_a.is_number())
							{
								auto a = color_a.get<float>();
								vertex.a = (uint8_t)std::round(a * 255.0f);
							}
						}

						vertexArray[vertexArrayCount] = vertex;
						vertexArrayCount++;
					}
					else
					{
						//TODO: Log Parse Error
					}
				}
			}
			else
			{
				//TODO: Log Parse Error
			}
		}
		
		//Parse Index Data
		{
			const auto indexDataArray = parsedFile["index_data"];
			if (indexDataArray.is_array())
			{
				for (const auto& indexData : indexDataArray)
				{
					if (indexData.is_number())
					{
						indexArray[indexArrayCount] = indexData;
						indexArrayCount++;
					}
					else
					{
						//TODO: Log Parse Error
					}
				}
			}
			else
			{
				//TODO: Log Parse Error
			}
		}		
	}


	// Initialize the platform-specific mesh
	if (!(result = newMesh->Initialize(vertexArray, vertexArrayCount, indexArray, indexArrayCount)))
	{
		EAE6320_ASSERTF(false, "Initialization of new mesh failed");
		return result;
	}

	delete[] vertexArray;
	delete[] indexArray;

	return result;
}

eae6320::Graphics::cMesh::~cMesh()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}