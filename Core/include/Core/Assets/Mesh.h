/* 
* Class Adapted from
* https://github.com/Eikins/3D-Sea-Project/blob/master/sea3d/core/mesh.py#L118
* Author : Noe M.
*/
#pragma once

#include <vector>

#include "Math/Math.h"
#include "Asset.h"

namespace Core
{
	struct VertexData
	{
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector4 tangent; // Mikkt-Space tangents
		Math::Vector2 texCoords;
	};

	class Mesh : public Asset
	{
	private:
		uint16_t _vertexCount = 0;
		std::vector<Math::Vector3> _vertices;
		std::vector<Math::Vector3> _normals;
		std::vector<Math::Vector4> _tangents; // Mikkt-Space tangents
		std::vector<Math::Vector2> _texCoords;
		
		std::vector<uint16_t> _indices;

		Mesh() : Asset("") {}
	public:
		Mesh(
			std::string name,
			std::vector<Math::Vector3> vertices,
			std::vector<Math::Vector3> normals,
			std::vector<Math::Vector4> tangents,
			std::vector<Math::Vector2> texCoords,
			std::vector<uint16_t> indices
		);

		Mesh(
			std::string name,
			std::vector<Math::Vector3> vertices,
			std::vector<Math::Vector3> normals,
			std::vector<Math::Vector2> texCoords,
			std::vector<uint16_t> indices
		);

		uint16_t GetVertexCount();

		void ComputeTangents();
		std::vector<VertexData> GenerateInterleavedData();
		std::vector<uint16_t> GetIndices();

		static std::vector<Mesh> LoadFromFile(const std::string path, bool flipUVs = true, bool generateNormals = true, bool fixNormals = true);

		static Mesh Transform(const Mesh& mesh, const Math::Matrix4x4& matrix);
	};
}