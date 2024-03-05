#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <Types.h>

/**
 * @class Texture
 * @brief Manages texture resources.
 *
 * This class encapsulates texture resources, including loading textures from files 
 * and ensuring textures are unique through a caching mechanism.
 * 
 */
class Texture
{
    TextureID mId{ 0U };
    std::string mPath;

public:
    static Texture* Load(const std::string& path);
    static void Unload(const std::string& path);
    static void ClearCache();
    
    TextureID GetTextureID() const { return mId; };
    bool IsValid() const { return mId == 0; };

private:
    Texture(const std::string& path);
    ~Texture();

    void LoadTexture();

    static std::unordered_map<std::string, Texture*> mCache;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&&) = delete;
    Texture& operator=(Texture&&) = delete;
};
