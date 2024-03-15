#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>

class FBXLoader
{
public:
    static bool LoadFBX(const char* filePath);

private:
    static void ProcessNode(FbxNode* node);
    static void ProcessMesh(FbxMesh* mesh);
};