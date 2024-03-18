#pragma once
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <iostream>

template <typename T>
inline T GetProperty(const std::string& key, aiMaterial* material)
{
	T value;
	material->Get(key.c_str(), 0, 0, value);

	return value;
}

// Returns worldspace transformation matrix
inline aiMatrix4x4 GetTransformationMatrix(const aiNode* node)
{
	if (node->mParent == nullptr)
	{ // Root node
		return node->mTransformation;
	}
	else
	{
		return node->mParent->mTransformation * GetTransformationMatrix(node->mParent);
	}
}

inline aiVector3D TransformPosition(const aiMatrix4x4& transform, const aiVector3D& position)
{
	aiVector3D result;
	result.x = transform.a1 * position.x + transform.a2 * position.y + transform.a3 * position.z + transform.a4;
	result.y = transform.b1 * position.x + transform.b2 * position.y + transform.b3 * position.z + transform.b4;
	result.z = transform.c1 * position.x + transform.c2 * position.y + transform.c3 * position.z + transform.c4;
	return result;
}

inline void PrintMatrix(const aiMatrix4x4& mat) {
	std::cout << mat.a1 << " " << mat.a2 << " " << mat.a3 << " " << mat.a4 << std::endl;
	std::cout << mat.b1 << " " << mat.b2 << " " << mat.b3 << " " << mat.b4 << std::endl;
	std::cout << mat.c1 << " " << mat.c2 << " " << mat.c3 << " " << mat.c4 << std::endl;
	std::cout << mat.d1 << " " << mat.d2 << " " << mat.d3 << " " << mat.d4 << std::endl;
}

inline void PrintVector3D(const aiVector3D& vec) 
{
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

inline glm::mat4 AiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
	glm::mat4 to;

	to[0][0] = from.a1; to[1][0] = from.a2;
	to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2;
	to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2;
	to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2;
	to[2][3] = from.d3; to[3][3] = from.d4;

	return to;
}
