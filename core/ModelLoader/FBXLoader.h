#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#include <string>

class FBXLoader
{
public:
    static bool LoadFBX(const std::string& path, class Actor* staticMeshActor);

private:
    static void ProcessNode(FbxNode* node, class Actor* actor);
    static void ProcessMesh(FbxMesh* mesh, class Actor* actor);
    static class Mesh* ProcessPolygons(FbxMesh* mesh);
    static class Material* ProcessMaterial(FbxSurfaceMaterial* material);
    static std::string msBasePath;
    static long long msNameIndex;
};