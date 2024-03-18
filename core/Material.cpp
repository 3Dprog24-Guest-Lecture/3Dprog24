#include "Material.h"

std::unordered_map<std::string, Material*> Material::msCache;

Material::Material(const std::string& name)
{
    mTextures.fill(nullptr);
}

Material* Material::Load(const std::string& name)
{
    return Load(name, {}, {});
}

Material* Material::Load(const std::string& name, const std::array<Texture*, TextureType::COUNT>& textures, const MaterialProperties& properties)
{
    auto it = msCache.find(name);
    if (it != msCache.end())
    {
        return msCache[name];
    }

    Material* material = new Material(name);
    material->mTextures = textures;
    material->mProperties = properties;
    msCache[name] = material;
    return material;
}

void Material::Unload(const std::string& name)
{
    auto it = msCache.find(name);
    if (it != msCache.end())
    {
        delete it->second;
        msCache.erase(it);
    }
}

void Material::ClearCache()
{
    for (auto it : msCache)
    {
        delete it.second;
    }
    msCache.clear();
}

void Material::Bind(const Shader* shader) const
{
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        if (mTextures[i])
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mTextures[i]->GetTextureID());

            shader->setInt(textureUniformNames[i], static_cast<int>(i));
        }
    }

    shader->setVec3("material.diffuseColor", mProperties.mDiffuse);
    shader->setFloat("material.shininess", mProperties.mShininess);
}

void Material::SetTexture(TextureType type, Texture* texture)
{
    if (type >= 0 && type < TextureType::COUNT)
    {
        mTextures[type] = texture;
    }
}

Texture* Material::GetTexture(TextureType type) const
{
    if (type >= 0 && type < TextureType::COUNT)
    {
        return mTextures[type];
    }
    return nullptr;
}