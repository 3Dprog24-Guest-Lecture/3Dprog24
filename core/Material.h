#pragma once

#include <Shader.h>
#include <Texture.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <array>


class Material
{
public:
    // New types and order must match the array below
    enum TextureType
    {
        ALBEDO,
        SPECULAR,
        NORMAL,
        ALPHA,
        COUNT
    };

    std::array<std::string, static_cast<size_t>(TextureType::COUNT)> textureUniformNames = {   
        "material.albedoMap", "material.specularMap", 
        "material.normalMap", "material.alphaMap"
    };

    // Update the Bind() function when adding new variables
    struct MaterialProperties
    {
        glm::vec3 mColor{1.f, 1.f, 1.f};
        float mShininess{ 1.f };
    };

private:
    MaterialProperties mProperties{};
    std::array<Texture*, TextureType::COUNT> mTextures{};
    static std::unordered_map<std::string, Material*> mCache;

    Material(const std::string& name);

public:
    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;

    static Material* Load(const std::string& name);
    static Material* Load(const std::string& name, const std::array<Texture*, TextureType::COUNT>& textures, const MaterialProperties& properties);
    static void Unload(const std::string& name);
    static void ClearCache() { mCache.clear(); };

    // Assumes the shader is bound
    void Bind(const Shader* shader) const;

    void SetTexture(TextureType type, Texture* texture);
    Texture* GetTexture(TextureType type) const;

    const MaterialProperties& GetProperties() const { return mProperties; }
    void SetProperties(const MaterialProperties& properties) { mProperties = properties; }
};