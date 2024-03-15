#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#include <string>

class FBXLoader
{
public:
    static bool LoadFBX(const std::string& path);

private:
    static void ProcessNode(FbxNode* node);
    static void ProcessMesh(FbxMesh* mesh);
};