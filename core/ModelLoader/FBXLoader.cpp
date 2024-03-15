#include <ModelLoader/FBXLoader.h>
#include <Logger.h>
#include <exception>

bool FBXLoader::LoadFBX(const std::string& path)
{
    LOG_INFO("FBXLoader::LoadFBX %s", path.c_str());

    FbxManager* lSdkManager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

    if (!lImporter->Initialize(path.c_str(), -1, lSdkManager->GetIOSettings()))
    {
        throw std::runtime_error("Failed to initialize FBX mesh");
    }

    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
    lImporter->Import(lScene);
    lImporter->Destroy();

    ProcessNode(lScene->GetRootNode());

    lSdkManager->Destroy();
    return true;
}

void FBXLoader::ProcessNode(FbxNode* node)
{
    if (node == nullptr) return;

    for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    {
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
        if (attribute == nullptr) continue;

        if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {           
            ProcessMesh(node->GetMesh());
        }
    }

    for (int j = 0; j < node->GetChildCount(); j++)
    {
        ProcessNode(node->GetChild(j));
    }
}

void FBXLoader::ProcessMesh(FbxMesh* mesh)
{
    LOG_INFO("FBXLoader::ProcessMesh::Mesh::%s", mesh->GetNameOnly().Buffer());
   
    if (mesh == nullptr) return;

    int numPolygons = mesh->GetPolygonCount();
    std::cout << "Number of polygons: " << numPolygons << std::endl;

    int materialCount = mesh->GetNode()->GetMaterialCount();
    for (int i = 0; i < materialCount; i++)
    {
        FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(i);

        if (material != nullptr)
        {
            
            FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            int textureCount = prop.GetSrcObjectCount<FbxTexture>();
            for (int j = 0; j < textureCount; j++)
            {
                FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(j));
                if (texture)
                {
                    std::cout << "Diffuse Texture Name: " << texture->GetName() << std::endl;
                }
            }           
        }
    }

}

