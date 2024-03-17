#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssimpLoader
{
public:
    static void Load(const std::string& path, class Actor* staticMeshActor, long long flags);

private:
    static void ProcessLights(const class aiScene* scene, class Actor* actor);
    static void ProcessNode(const class aiScene* scene, class aiNode* node, class Actor* actor);   
    static class Mesh* ProcessMesh(class aiMesh* mesh);
    static class Material* ProcessMaterial(class aiMaterial* material);
    static class AABBActor* ProcessCollisionAABB(class aiMesh* mesh);
    static std::string msBasePath;
    static long long msNameIndex;
    static long long msFlags;
};