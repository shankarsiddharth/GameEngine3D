// Includes
//=========

#include "cMeshBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>
#include <Engine/Graphics/VertexFormats.h>
#include <External/JSON/Includes.h>

#include <fstream>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = Results::Success;

	// Read the json file
	eae6320::Platform::sDataFromFile dataFromFile;
	eae6320::Platform::LoadBinaryFile(m_path_source, dataFromFile);

	const auto parsedFile = nlohmann::json::parse(static_cast<const char*>(dataFromFile.data),
		static_cast<const char*>(dataFromFile.data) + dataFromFile.size);

	uint16_t vertexArrayIndex = 0;
	uint16_t indexArrayIndex = 0;

	constexpr uint16_t maxVertexCount = std::numeric_limits<uint16_t>::max();

	// TODO: Error Check max size and print log information and error
	auto* vertexArray = new eae6320::Graphics::VertexFormats::sVertex_mesh[maxVertexCount];
	auto* indexArray = new uint16_t[maxVertexCount];


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

						if (vertexArrayIndex < maxVertexCount)
						{
							vertexArray[vertexArrayIndex] = vertex;
							vertexArrayIndex++;
						}
						else
						{
							//TODO: Log Vertex Count Exceeded / Mesh File Too Large to load
							result = Results::OutOfMemory;
							std::string errorMessage = "Couldn't process mesh file \"" + std::string(m_path_source) + "\", vertex/index count/size larger than 65535.";
							OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());
							break;
						}

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
		if (result == Results::Success)
		{
			const auto indexDataArray = parsedFile["index_data"];
			if (indexDataArray.is_array())
			{
				for (const auto& indexData : indexDataArray)
				{
					if (indexData.is_number())
					{
						indexArray[indexArrayIndex] = indexData;
						indexArrayIndex++;
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

	if (result == Results::OutOfMemory)
	{
		//Error: Mesh file contains too many vertices to process
		std::string errorMessage = "Mesh file contains too many vertices to process. Couldn't process mesh file \"" + std::string(m_path_source) +"\", vertex/index count/size larger than 65535.";
		OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());
	}
	else
	{
		// #dev08 TODO: Write Binary Data to File
		std::ofstream outBinaryFile(m_path_target, std::ofstream::binary);		
		outBinaryFile.write(reinterpret_cast<char*>(&vertexArrayIndex), sizeof(uint16_t));
		outBinaryFile.write(reinterpret_cast<char*>(&indexArrayIndex), sizeof(uint16_t));
		outBinaryFile.write(reinterpret_cast<char*>(vertexArray), vertexArrayIndex * sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh));
		outBinaryFile.write(reinterpret_cast<char*>(indexArray), indexArrayIndex * sizeof(uint16_t));
		outBinaryFile.close();
	}

	delete[] vertexArray;
	vertexArray = nullptr;
	delete[] indexArray;
	indexArray = nullptr;

	return result;
}
