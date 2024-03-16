#include <ModelLoader/FBXLoader.h>
#include <Logger.h>
#include <exception>
#include <filesystem>
#include <Actor.h>
#include <Mesh.h>
#include <Vertex.h>
#include <Material.h>

namespace fs = std::filesystem;

std::string FBXLoader::msBasePath = "";
long long FBXLoader::msNameIndex = 0;

std::string ToRelativePath(const std::string& basePath, const std::string& absolutePath) {
    fs::path base(basePath);
    fs::path absolute(absolutePath);
    fs::path relative = fs::relative(absolute, base);
    return relative.string();
}

std::string GetFileNameFromPath(const std::string& filePath) 
{
    size_t pos = filePath.find_last_of("\\/");
    if (pos != std::string::npos) 
    {
        return filePath.substr(pos + 1);
    }
    return filePath;
}

std::string GetDirectoryPath(const std::string& filePath) 
{
    size_t pos = filePath.find_last_of("\\/");
    if (pos != std::string::npos) 
    {
        return filePath.substr(0, pos + 1);
    }
    return "";
}

std::string GetFileExtension(const std::string& filePath) {
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos) 
    {
        return filePath.substr(pos);
    }
    return "";
}

std::string RemoveFileExtension(const std::string& filePath)
{
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos) 
    {
        return filePath.substr(0, pos);
    }
    return filePath;
}

bool FBXLoader::LoadFBX(const std::string& path, Actor* staticMeshActor)
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

    msBasePath = path;
    ProcessNode(lScene->GetRootNode(), staticMeshActor);

    lSdkManager->Destroy();
    return true;
}

void FBXLoader::ProcessNode(FbxNode* node, Actor* actor)
{
    if (node == nullptr) return;

    for (int i = 0; i < node->GetNodeAttributeCount(); i++)
    {
        FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);
        if (attribute == nullptr) continue;

        if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {           
            ProcessMesh(node->GetMesh(), actor);
        }
    }

    for (int j = 0; j < node->GetChildCount(); j++)
    {
        ProcessNode(node->GetChild(j), actor);
    }
}

void FBXLoader::ProcessMesh(FbxMesh* mesh, Actor* actor)
{
    LOG_INFO("FBXLoader::ProcessMesh::Name %s", mesh->GetNameOnly().Buffer());
   
    if (mesh == nullptr) return;

    int numPolygons = mesh->GetPolygonCount();
    LOG_INFO("FBXLoader::ProcessMesh::NumPoly: %i", numPolygons);

    Mesh* internalMesh = ProcessPolygons(mesh);

    int materialCount = mesh->GetNode()->GetMaterialCount();
    LOG_INFO("FBXLoader::ProcessMesh::MaterialCount: %i", materialCount);
    if (materialCount > 0)
    {
        // Only support for 1 material per mesh
        FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(0);
        internalMesh->SetMaterial(ProcessMaterial(material));
    }

    std::string modelName = RemoveFileExtension(GetFileNameFromPath(msBasePath));
    std::string meshName = mesh->GetName();
    std::string meshActorName = modelName + "/" + "MeshLoadID: " + std::to_string(msNameIndex++) + meshName;

    auto meshActor = new MeshActor(meshActorName, internalMesh);
    LOG_INFO("FBXLoader::ProcessMesh::MeshActorCreated");
    actor->AddChild(meshActor);
    LOG_INFO("---------------------------------------------------------------\n");
}

Mesh* FBXLoader::ProcessPolygons(FbxMesh* mesh)
{
    if (!mesh) return nullptr;

    LOG_INFO("FBXLoader::ProcessPolygons");

    const int polygonCount = mesh->GetPolygonCount();

    FbxVector4* controlPoints = mesh->GetControlPoints();
    FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
    FbxGeometryElementUV* uvElement = mesh->GetElementUV();

    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    for (int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); ++polygonIndex) 
    {
        const int polygonSize = 3; // Assuming 3 for triangles

        for (int vertexPosition = 0; vertexPosition < polygonSize; ++vertexPosition) 
        {
            int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, vertexPosition);

            glm::vec3 position = glm::vec3(
                controlPoints[controlPointIndex][0],
                controlPoints[controlPointIndex][1],
                controlPoints[controlPointIndex][2]
            );

            glm::vec3 normal(0.0f, 0.0f, 0.0f);
            if (normalElement) 
            {
                int normalIndex = polygonIndex * polygonSize + vertexPosition;
                if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) 
                {
                    if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) 
                    {
                        normal = glm::vec3(
                            normalElement->GetDirectArray().GetAt(normalIndex)[0],
                            normalElement->GetDirectArray().GetAt(normalIndex)[1],
                            normalElement->GetDirectArray().GetAt(normalIndex)[2]);
                    }
                    else if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) 
                    {
                        int id = normalElement->GetIndexArray().GetAt(normalIndex);
                        normal = glm::vec3(
                            normalElement->GetDirectArray().GetAt(id)[0],
                            normalElement->GetDirectArray().GetAt(id)[1],
                            normalElement->GetDirectArray().GetAt(id)[2]);
                    }
                }
            }

            glm::vec2 uv(0.0f, 0.0f);
            if (uvElement && uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) 
            {
                FbxVector2 fbxUV;
                bool unmappedUV;
                mesh->GetPolygonVertexUV(polygonIndex, vertexPosition, uvElement->GetName(), fbxUV, unmappedUV);
                uv = glm::vec2(fbxUV[0], fbxUV[1]);
            }
            vertices.emplace_back(position, normal, uv);
            // For each vertex of the polygon, add an index
            indices.push_back(indices.size());
        }
    }
    return new Mesh(std::string(mesh->GetName()), std::move(vertices), std::move(indices), nullptr);
}

Material* FBXLoader::ProcessMaterial(FbxSurfaceMaterial* material)
{
    if (!material) return nullptr;

    LOG_INFO("FBXLoader::ProcessMaterial");

    auto internalMaterial = Material::Load(std::string(material->GetName()), {}, {});

    // Process diffuse textures
    FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
    int textureCount = prop.GetSrcObjectCount<FbxTexture>();
    LOG_INFO("FBXLoader::ProcessMaterial::Diffuse::TextureCount: %i", textureCount);
    for (int j = 0; j < textureCount; j++) 
    {
        FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(j));
        if (texture) 
        {
            LOG_INFO("FBXLoader::ProcessMaterial::Diffuse::Texture::Name: %s", texture->GetName());
            FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
            if (fileTexture) 
            {
                std::string baseDirectory = GetDirectoryPath(msBasePath);
                std::string textureName = GetFileNameFromPath(fileTexture->GetFileName());
                std::string textureFilePath = baseDirectory + "Textures/" + textureName;
                LOG_INFO("FBXLoader::ProcessMesh::Material::Diffuse::Texture::FilePath: %s", textureFilePath.c_str());

                internalMaterial->SetTexture(Material::DIFFUSE, Texture::Load(textureFilePath));
            }
        }
    }

    Material::MaterialProperties internalMaterialProperties;

    // Process diffuse color
    FbxDouble3 diffuseColor = material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor).Get<FbxDouble3>();
    LOG_INFO("FBXLoader::ProcessMaterial::DiffuseColor: (%f, %f, %f)",
        diffuseColor[0], diffuseColor[1], diffuseColor[2]);

    internalMaterialProperties.mColor = {diffuseColor[0], diffuseColor[1], diffuseColor[2]};
    internalMaterial->SetProperties(internalMaterialProperties);

    return internalMaterial;
}