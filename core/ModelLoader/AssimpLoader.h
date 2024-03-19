#pragma once

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/**
 * @brief class for loading models using Assimp.
 *
 * This class provides static methods for loading models from a file, processing
 * the Assimp scene, nodes, meshes, materials, and generating collision bounding
 * boxes.
 */
class AssimpLoader
{
public:
    /**
     * @brief Loads a model from a file and attaches it to a given actor.
     *
     * @param path Path to the model file.
     * @param staticMeshActor The root of the scene graph.
     * @param flags Import flags (default is aimed at realtime max quality).
     */
    static void Load(const std::string& path, class Actor* staticMeshActor, unsigned flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);

private:
    /**
     * @brief Processes lighting information such as pointlights.
     *
     * @param scene Assimp scene containing the lighting information.
     * @param rootActor Root Actor to attach the pointlights to.
     */
    static void ProcessLights(const class aiScene* scene, class Actor* rootActor);

    /**
     * @brief Recursively processes an Assimp node and its children.
     *
     * @param scene Assimp scene containing the data.
     * @param node Assimp node to process meshes associated with the node.
     * @param parentActor Actor to attach processed meshes to.
     */
    static void ProcessNode(const class aiScene* scene, class aiNode* node, class Actor* parentActor);

    /**
     * @brief Processes an Assimp mesh and converts it into a Mesh class instance.
     *
     * @param mesh Assimp mesh to process.
     * @return A new Mesh instance containing the processed data.
     */
    static class Mesh* ProcessMesh(class aiMesh* mesh);

    /**
     * @brief Processes an Assimp material and converts it into a Material class instance.
     *
     * @param material Assimp material to process.
     * @return A new Material instance containing the processed data.
     */
    static class Material* ProcessMaterial(class aiMaterial* material);

    /**
     * Generates an axis-aligned bounding box for collision detection from an Assimp mesh.
     *
     * @param[in] mesh Assimp mesh to process.
     * @return An AABB instance representing the bounding box.
     */
    static class AABB ProcessCollisionAABB(class aiMesh* mesh);

    /**
    * @brief Base path to the fbx/* file.
    */
    static std::string msBasePath;

    /**
    * @brief Name index for generating unique names.
    */
    static long long msNameIndex;

    /**
    * @brief Flags used during the import process.
    */
    static long long msFlags;
};