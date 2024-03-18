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
#include <AABB.h>
#include <Collision.h>

/**
 * @class Mesh
 * @brief Represents a 3D mesh.
 *
 * Has the necessary data to render a 3D object, including vertices, indices,
 * and material information, but not a transform. Uses the lightweight pattern, copies and move operations are deleted.
 */
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

    // Assumes the shader is bound
    void Draw(const Shader* shader) const;

    static Mesh* LoadCube(Material* material);
    static Mesh* Load(const std::string& path);
    static void Unload(const std::string& path);
    static void ClearCache();

    void SetMaterial(Material* material) { mMaterial = material; }
    const Material* GetMaterial() const { return mMaterial; }

    static std::unordered_map<std::string, Mesh*> sCache;
};

/**
 * @class MeshActor
 * @brief Represents an actor in the scene that holds a mesh.
 *
 * MeshActor is a specialized type of actor designed to hold a mesh and render it.
 * It inherits from the Actor class to integrate into the scene graph and implements
 * the IRender interface to provide rendering functionality. This allows the MeshActor
 * to be managed within the scene graph while also having the capability to render a
 * specific mesh using a shader.
 */
class MeshActor : public Actor, public IRender, public IBounded
{
public:
    MeshActor(const std::string& name, Mesh* mesh);
    virtual void Draw(const class Shader* shader = nullptr) const override;
    virtual AABB GetAABB() const override;
    virtual CollisionProperties GetCollisionProperties() const override;
    
    Mesh* mMesh{ nullptr };
    CollisionProperties mCollisionProperties{CollisionType::STATIC, CollisionResponse::BLOCK };
};

class StaticMeshActor : public MeshActor
{
public:
    StaticMeshActor(const std::string& name, Mesh* mesh)
        :MeshActor(name, mesh){};
};

// Can eventually create a MeshComponent
//class MeshComponent : public Component, public IRender