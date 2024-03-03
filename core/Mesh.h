#pragma once

#include <Renderer.h>
#include <Material.h>
#include <Shader.h>
#include <Vertex.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <Actor.h>
#include <Types.h>

class Mesh 
{
private:
    std::vector<Vertex> mVertices{};
    std::vector<Index> mIndices{};
    Material* mMaterial{nullptr};

    VAO mVAO{ 0U };
    VBO mVBO{ 0U };
    EBO mEBO{ 0U };

    void SetupMesh();

public:
    explicit Mesh(const std::string& name, std::vector<Vertex>&& vertices, std::vector<Index>&& indices, Material* material);
    virtual ~Mesh();

    void Draw(const Shader* shader) const;

    static Mesh* CreateCube(Material* material);
    static Mesh* Load(const std::string& path);
    static void Unload(const std::string& path);
    static void ClearCache() { mCache.clear(); };

    static std::unordered_map<std::string, Mesh*> mCache;
};

class MeshActor : public Actor, public IRender
{
public:
    MeshActor(const std::string& name, Mesh* mesh);
    virtual void Draw(const class Shader* shader = nullptr) const;
    Mesh* mMesh{ nullptr };
};

// Can eventually create a MeshComponent
//class MeshComponent : public Component, public IRender