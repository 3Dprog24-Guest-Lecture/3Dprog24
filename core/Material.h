#pragma once

#include <Shader.h>
#include <Texture.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <array>

/**
 * @class Material
 * @brief Manages material properties and textures for rendering.
 *
 * @note This class cannot be copied or assigned due to the deletion of the copy
 * constructor and copy assignment operator, ensuring unique instances for the use of the lightweight pattern.
 */
class Material
{
public:
    // New types and order must match the array below
    enum TextureType
    {
        DIFFUSE, SPECULAR, NORMAL, ALPHA, COUNT
    };

    std::array<std::string, static_cast<size_t>(TextureType::COUNT)> textureUniformNames = {   
        "material.diffuseMap", "material.specularMap", 
        "material.normalMap", "material.alphaMap"
    };

    /**
     * @struct MaterialProperties
     * @brief Holds properties related to the visual appearance of a material.
     *
     * This struct encapsulates properties that define the visual characteristics of a material,
     * including its base color and shininess, i.e (object final color * mColor).
     * 
     * @note Update the Bind() function when adding new variables
     */
    struct MaterialProperties
    {
        glm::vec3 mAmbient{ 1.f, 1.f, 1.f };
        glm::vec3 mDiffuse{1.f, 1.f, 1.f};
        glm::vec3 mSpecular{ 1.f, 1.f, 1.f };
        float mShininess{ 64.f };
        float mAlphaCutoff = 1.f;
    };

private:
    MaterialProperties mProperties{};
    std::array<Texture*, TextureType::COUNT> mTextures{};
    static std::unordered_map<std::string, Material*> msCache;

    Material(const std::string& name);

public:
    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;

    Material(Material&&) = delete;
    Material& operator=(Material&&) = delete;

    static Material* Load(const std::string& name);
    static Material* Load(const std::string& name, const std::array<Texture*, TextureType::COUNT>& textures, const MaterialProperties& properties);
    static void Unload(const std::string& name);
    static void ClearCache();

    // Assumes the shader is bound
    void Bind(const Shader* shader) const;

    void SetTexture(TextureType type, Texture* texture);
    Texture* GetTexture(TextureType type) const;

    const MaterialProperties& GetProperties() const { return mProperties; }
    void SetProperties(const MaterialProperties& properties) { mProperties = properties; }
};