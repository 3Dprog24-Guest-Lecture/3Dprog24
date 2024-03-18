#include <ModelLoader/AssimpLoader.h>
#include <ModelLoader/Shared.h>
#include <Logger.h>
#include <Vertex.h>
#include <Mesh.h>
#include <Material.h>
#include <Texture.h>
#include <exception>
#include <Lights/PointLight.h>
#include <ModelLoader/AssimpUtility.h>

std::string AssimpLoader::msBasePath = "";
long long AssimpLoader::msNameIndex = 0;
long long AssimpLoader::msFlags = 0;

void AssimpLoader::Load(const std::string& path, Actor* staticMeshActor, unsigned flags)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_INFO("AssimpLoader::Load::Failed: %s", importer.GetErrorString());
		throw std::runtime_error("AssimpLoader::Load::Failed");
	}

	msBasePath = path;
	msFlags = flags;
	ProcessLights(scene, staticMeshActor);
	ProcessNode(scene, scene->mRootNode, staticMeshActor);
}

void AssimpLoader::ProcessLights(const aiScene* scene, Actor* actor)
{
	LOG_INFO("AssimpLoader::ProcessLights::NumLights: %i", scene->mNumLights);
	for (size_t i = 0; i < scene->mNumLights; i++)
	{
		if (scene->mLights[i]->mType != aiLightSource_POINT)
			continue;

		auto pointLightActor = new PointLightActor(scene->mLights[i]->mName.C_Str());

		auto lightNode = scene->mRootNode->FindNode(scene->mLights[i]->mName);
		auto lightTransform = GetTransformationMatrix(lightNode);
		aiVector3D worldPosition = TransformPosition(lightTransform, scene->mLights[i]->mPosition);

		PrintMatrix(lightTransform);
		PrintVector3D(worldPosition);

		glm::vec3 pos = 
		{ 
			worldPosition.x,
			worldPosition.y,
			worldPosition.z
		};
		pointLightActor->mAmbient =
		{
			scene->mLights[i]->mColorAmbient.r,
			scene->mLights[i]->mColorAmbient.g,
			scene->mLights[i]->mColorAmbient.b
		};
		pointLightActor->mColor =
		{
			scene->mLights[i]->mColorDiffuse.r,
			scene->mLights[i]->mColorDiffuse.g,
			scene->mLights[i]->mColorDiffuse.b
		};

		pointLightActor->mColor = glm::clamp(pointLightActor->mColor, 0.f, 1.f);
		pointLightActor->SetWorldPosition(pos);
		actor->AddChild(pointLightActor);
	}
}

void AssimpLoader::ProcessNode(const aiScene* scene, aiNode* node, Actor* parentActor)
{
	Actor* actor = nullptr;
	for (auto i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		std::string collisionPrefix;
		std::string lightPrefix;
		if (HasCollisionPrefix(mesh->mName.C_Str(), collisionPrefix))
		{
			LOG_INFO("AssimpLoader::ProcessNode::CollisionPrefix: %s", collisionPrefix.c_str());
			actor = ProcessCollisionAABB(mesh);
		}
		else if (HasLightPrefix(mesh->mName.C_Str(), lightPrefix))
		{
			LOG_INFO("AssimpLoader::ProcessNode::LightPrefix: %s", lightPrefix.c_str());
		}
		else
		{ 
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			auto internalMesh = ProcessMesh(mesh);
			auto internalMaterial = ProcessMaterial(material);
			internalMesh->SetMaterial(internalMaterial);

			std::string modelName = RemoveFileExtension(GetFileNameFromPath(msBasePath));
			std::string meshName = mesh->mName.C_Str();
			std::string meshActorName = modelName + "/" + "MeshLoadID: " + std::to_string(msNameIndex++) + meshName;
			MeshActor* meshActor = new MeshActor(meshActorName, internalMesh);
			meshActor->mCollisionProperties.mResponse = CollisionResponse::IGNORE;
			actor = meshActor;
		}
	}
	if (!actor)
	{
		actor = new Actor(std::string(node->mName.C_Str()) + "NodeID: " + std::to_string(msNameIndex++));
	}
	
	{ // Apply node transform
		actor->SetLocalTransformMatrix(AiMatrix4x4ToGlm(node->mTransformation));
	}

	// Add node as a child
	parentActor->AddChild(actor);

	for (auto i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(scene, node->mChildren[i], actor);
	}
	LOG_INFO("----------------------------------------------------\n");
}

Mesh* AssimpLoader::ProcessMesh(aiMesh* mesh)
{
	LOG_INFO("AssimpLoader::ProcessMesh::Name: %s", mesh->mName.C_Str());

	std::vector<Vertex> vertices;
	std::vector<Index> indices;

	for (auto i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 pos{ 0.f };
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;

		glm::vec3 normal{ 0.f, 1.f, 0.f };
		if (mesh->HasNormals())
		{
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
		}
		glm::vec2 texCoords{0.f};
		if (mesh->HasTextureCoords(0))
		{
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back({ pos, normal, texCoords });
	}

	for (auto i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (auto j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return new Mesh(std::string(mesh->mName.C_Str()), std::move(vertices), std::move(indices), nullptr);
}

Material* AssimpLoader::ProcessMaterial(aiMaterial* material)
{
	if (!material) return nullptr;

	LOG_INFO("AssimpLoader::ProcessMaterial::Name: %s", material->GetName().C_Str());

	auto internalMaterial = Material::Load(std::string(material->GetName().C_Str()), {}, {});

	internalMaterial->SetTexture(Material::DIFFUSE, Texture::LoadWhiteTexture());
	internalMaterial->SetTexture(Material::SPECULAR, Texture::LoadBlackTexture());

	// Diffuse
	if (0 < material->GetTextureCount(aiTextureType_DIFFUSE))
	{
		aiString str;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		std::string texturePath = GetDirectoryPath(msBasePath) + "Textures/" + std::string(str.C_Str());
		LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Diffuse::Path: %s", texturePath.c_str());
		internalMaterial->SetTexture(Material::DIFFUSE, Texture::Load(texturePath));
	}
	// Specular
	if (0 < material->GetTextureCount(aiTextureType_SPECULAR))
	{
		aiString str;
		material->GetTexture(aiTextureType_SPECULAR, 0, &str);
		std::string texturePath = GetDirectoryPath(msBasePath) + "Textures/" + std::string(str.C_Str());
		LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Specular::Path: %s", texturePath.c_str());
		internalMaterial->SetTexture(Material::SPECULAR, Texture::Load(texturePath));
	}
	// Normal
	if (0 < material->GetTextureCount(aiTextureType_NORMALS))
	{
		aiString str;
		material->GetTexture(aiTextureType_NORMALS, 0, &str);
		std::string texturePath = GetDirectoryPath(msBasePath) + "Textures/" + std::string(str.C_Str());
		LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Normal::Path: %s", texturePath.c_str());
		internalMaterial->SetTexture(Material::NORMAL, Texture::Load(texturePath));
	}
	// Opacity
	if (0 < material->GetTextureCount(aiTextureType_OPACITY))
	{
		aiString str;
		material->GetTexture(aiTextureType_OPACITY, 0, &str);
		std::string texturePath = GetDirectoryPath(msBasePath) + "Textures/" + std::string(str.C_Str());
		LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Opacity::Path: %s", texturePath.c_str());
		internalMaterial->SetTexture(Material::ALPHA, Texture::Load(texturePath));
	}

	//Material::MaterialProperties materialProperties;
	//materialProperties.mAlphaCutoff = GetProperty<float>("$mat.gltf.alphaCutoff", material);
	//materialProperties.mAmbient = GetProperty<glm::vec3>("$mat.ambient", material);
	//materialProperties.mDiffuse = GetProperty<glm::vec3>("$clr.diffuse", material);
	//materialProperties.mSpecular = GetProperty<glm::vec3>("$clr.specular", material);
	//internalMaterial->SetProperties(materialProperties);
		
	return internalMaterial;
}

AABBActor* AssimpLoader::ProcessCollisionAABB(aiMesh* mesh)
{
	AABB aabb{ {},{} };
	for (auto i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 pos{ 0.f };
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;

		aabb.center += pos;
	}
	aabb.center /= mesh->mNumVertices;

	for (auto i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 pos{ 0.f };
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		
		LOG_INFO("pos: (%f, %f, %f)", pos.x, pos.y, pos.z);

		aabb.Expand(pos);
	}

	LOG_INFO("aabb extent: (%f, %f, %f)", aabb.extent.x, aabb.extent.y, aabb.extent.z);
	const std::string actorName = std::string(mesh->mName.C_Str()) + std::to_string(msNameIndex++);
	return new AABBActor(actorName, aabb);
}
