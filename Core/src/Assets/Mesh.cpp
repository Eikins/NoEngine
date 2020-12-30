#include "Core/Assets/Mesh.h"

#include <stdexcept>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Core
{

	Mesh::Mesh(
		std::string name,
		std::vector<Math::Vector3> vertices,
		std::vector<Math::Vector3> normals,
		std::vector<Math::Vector4> tangents,
		std::vector<Math::Vector2> texCoords,
		std::vector<uint16_t> indices
	) : Asset(name)
	{
		bool allSizeEquals =
			vertices.size() == normals.size()
			&& vertices.size() == tangents.size()
			&& vertices.size() == texCoords.size();

		if (allSizeEquals == false)
		{
			throw std::runtime_error("Fatal Error : Trying to create a mesh without a consistent size across the data");
		}

		_vertexCount = vertices.size();
		_vertices = std::vector<Math::Vector3>(vertices);
		_normals = std::vector<Math::Vector3>(normals);
		_tangents = std::vector<Math::Vector4>(tangents);
		_texCoords = std::vector<Math::Vector2>(texCoords);
		_indices = std::vector<uint16_t>(indices);
	}

	Mesh::Mesh(
		std::string name,
		std::vector<Math::Vector3> vertices,
		std::vector<Math::Vector3> normals,
		std::vector<Math::Vector2> texCoords,
		std::vector<uint16_t> indices
	) : Asset(name)
	{
		bool allSizeEquals =
			vertices.size() == normals.size()
			&& vertices.size() == texCoords.size();

		if (allSizeEquals == false)
		{
			throw std::runtime_error("Fatal Error : Trying to create a mesh without a consistent size across the data");
		}

		_vertexCount = vertices.size();
		_vertices = std::vector<Math::Vector3>(vertices);
		_normals = std::vector<Math::Vector3>(normals);
		_texCoords = std::vector<Math::Vector2>(texCoords);
		_indices = std::vector<uint16_t>(indices);

		ComputeTangents();
	}

	uint16_t Mesh::GetVertexCount()
	{
		return _vertexCount;
	}

	void Mesh::ComputeTangents()
	{
		// Derived from stackoverflow answer:
		// https://gamedev.stackexchange.com/questions/68612/how-to-compute-tangent-and-bitangent-vectors

		_tangents.resize(_vertices.size(), Math::Vector4::Zero);
		// We also compute bitangent to define tangents in Mikkt-Space :
		// The tangent's w coordinate indicate weither the basis is left-handed or right-handed
		std::vector<Math::Vector3> bitangents(_tangents.size(), Math::Vector3::Zero);

		// First, accumulate tangent to have influence from all triangles
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
			// E = dUV * TB
			// T is the tangent vector
			// B is the bitangent vector
			
			// A = inverse(dUV)
			// Then B = A * E
			// Inverse dUV to compute A
			auto dUVdet = dUV1.x * dUV2.y - dUV1.y * dUV2.x;
			auto A1 = Math::Vector2(dUV2.y, -dUV2.x) / dUVdet;
			auto A2 = Math::Vector2(-dUV1.y, dUV1.x) / dUVdet;

			// Compute A * E
			auto T = Math::Vector3(
				Math::Vector2::Dot(A1, Math::Vector2(e1.x, e2.x)),
				Math::Vector2::Dot(A1, Math::Vector2(e1.y, e2.y)),
				Math::Vector2::Dot(A1, Math::Vector2(e1.z, e2.z))
			);

			auto B = Math::Vector3(
				Math::Vector2::Dot(A2, Math::Vector2(e1.x, e2.x)),
				Math::Vector2::Dot(A2, Math::Vector2(e1.y, e2.y)),
				Math::Vector2::Dot(A2, Math::Vector2(e1.z, e2.z))
			);

			_tangents[i] += Math::Vector4(T, 0.0F);
			bitangents[i] += B;
		}

		for (uint32_t i = 0; i < _tangents.size(); i++)
		{
			Math::Vector3 T = _tangents[i];
			Math::Vector3 B = bitangents[i];
			Math::Vector3 N = _normals[i];

			_tangents[i] = Math::Vector4(
				// Orthogonalize using Gram-Schmidt formula
				Math::Vector3::Normalize(T - N * Math::Vector3::Dot(T, N)),
				// Set Mikkt-Space handedness
				// Sign of the triple product of the basis (T, B, N)
				Math::Vector3::Dot(Math::Vector3::Cross(T, B), N) < 0.0F ? -1.0F : 1.0F
			);
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

	std::vector<uint16_t> Mesh::GetIndices()
	{
		return std::vector<uint16_t>(_indices);
	}

	Mesh Mesh::Transform(const Mesh& mesh, const Math::Matrix4x4& matrix)
	{
		Mesh tMesh(mesh.GetName(), mesh._vertices, mesh._normals, mesh._tangents, mesh._texCoords, mesh._indices);
		for (int i = 0; i < tMesh._vertexCount; i++)
		{
			tMesh._vertices[i] = matrix * Math::Vector4(tMesh._vertices[i], 1.0F);
			tMesh._normals[i] = matrix * Math::Vector4(tMesh._normals[i], 0.0F);
			tMesh._tangents[i] = matrix * Math::Vector4(tMesh._tangents[i], 0.0F);
		}
		return tMesh;
	}

	std::vector<Mesh> Mesh::LoadFromFile(const std::string path, bool flipUVs, bool generateNormals, bool fixNormals)
	{
		Assimp::Importer importer;

		unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded;
		if (generateNormals)
		{
			flags |= aiProcess_GenSmoothNormals;
		}
		if (flipUVs)
		{
			flags |= aiProcess_FlipUVs;
		}
		if (fixNormals)
		{
			flags |= aiProcess_FixInfacingNormals;
		}

		const aiScene* scene = importer.ReadFile(path.c_str(), flags);
		std::vector<Mesh> meshes(scene->mNumMeshes, Mesh());

		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto paiMesh = scene->mMeshes[i];
			Mesh& mesh = meshes[i];
			mesh._name = paiMesh->mName.C_Str();

			mesh._vertexCount = paiMesh->mNumVertices;
			mesh._vertices.resize(mesh._vertexCount, Math::Vector3::Zero);
			mesh._normals.resize(mesh._vertexCount, Math::Vector3::Zero);
			mesh._tangents.resize(mesh._vertexCount, Math::Vector4::Zero);
			mesh._texCoords.resize(mesh._vertexCount, Math::Vector2::Zero);
			for (uint32_t j = 0; j < paiMesh->mNumVertices; j++)
			{
				const aiVector3D vertex = paiMesh->mVertices[j];
				const aiVector3D normal = paiMesh->mNormals[j];
				const aiVector3D tangent = paiMesh->mTangents[j];
				const aiVector3D texCoords = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][j] : aiVector3D(0, 0, 0);

				mesh._vertices[j] = Math::Vector3(vertex.x, vertex.y, vertex.z);
				mesh._normals[j] = Math::Vector3(normal.x, normal.y, normal.z);
				// Post process already makes it left handed, so we just pass the mikkt component to 1
				mesh._tangents[j] = Math::Vector4(tangent.x, tangent.y, tangent.z, 1.0F);
				mesh._texCoords[j] = Math::Vector2(texCoords.x, texCoords.y);
			}

			mesh._indices.resize(paiMesh->mNumFaces * 3);
			for (uint32_t j = 0; j < paiMesh->mNumFaces; j++)
			{
				const aiFace face = paiMesh->mFaces[j];
				if (face.mNumIndices != 3)
				{
					throw std::runtime_error("Mesh is not valid, faces should be defined as triangles.");
				}
				mesh._indices[3 * j] = face.mIndices[0];
				mesh._indices[3 * j + 1] = face.mIndices[1];
				mesh._indices[3 * j + 2] = face.mIndices[2];
			}
		}
		return meshes;
	}
}