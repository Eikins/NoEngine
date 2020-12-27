/* 
* Class Adapted from
* https://github.com/Eikins/3D-Sea-Project/blob/master/sea3d/core/mesh.py#L118
* Author : Noe M.
*/
#pragma once

#include <vector>

#include "Math/Math.h"

namespace Core
{
	struct VertexData
	{
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector4 tangent; // Mikkt-Space tangents
		Math::Vector2 texCoords;
	};

	class Mesh
	{
	private:
		std::vector<Math::Vector3> _vertices;
		std::vector<Math::Vector3> _normals;
		std::vector<Math::Vector4> _tangents; // Mikkt-Space tangents
		std::vector<Math::Vector2> _texCoords;
		
		std::vector<uint16_t> _indices;
	public:
		void ComputeTangents();

		std::vector<VertexData> GenerateInterleavedData();

		static Mesh Transform(const Mesh& mesh, const Math::Matrix4x4& matrix);
	};
}