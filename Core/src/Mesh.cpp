#include "Core/Mesh.h"

namespace Core
{
	void Mesh::ComputeTangents()
	{
		_tangents.resize(_vertices.size(), Math::Vector4::Zero);
		for (uint32_t i = 0; i < _indices.size(); i += 3)
		{
			// For each triangle
			auto i0 = _indices[i];
			auto i1 = _indices[i + 1];
			auto i2 = _indices[i + 2];

			auto v0 = _vertices[i0];
			auto v1 = _vertices[i1];
			auto v2 = _vertices[i2];

			auto t0 = _texCoords[i0];
			auto t1 = _texCoords[i1];
			auto t2 = _texCoords[i2];

			// Get edges
			auto e1 = v1 - v0;
			auto e2 = v2 - v0;
			auto dUV1 = t1 - t0;
			auto dUV2 = t2 - t0;

			// We want to solve
			// | e1 |                   | T |
			// | e2 | = | dUV1 dUV2 | * | B |
		}

	}

	std::vector<VertexData> Mesh::GenerateInterleavedData()
	{
		uint32_t vertexCount = _vertices.size();
		std::vector<VertexData> data(vertexCount);
		
		for (uint32_t i = 0; i < vertexCount; i++)
		{
			VertexData vertexData{};
			vertexData.position = _vertices[i];
			vertexData.normal = _normals[i];
			vertexData.tangent = _tangents[i];
			vertexData.texCoords = _texCoords[i];
			data[i] = vertexData;
		}

		return data;
	}
}