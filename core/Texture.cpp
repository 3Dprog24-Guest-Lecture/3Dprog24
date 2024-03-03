#include <Texture.h>
#include <iostream>
#include <Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::unordered_map<std::string, Texture*> Texture::mCache;

Texture::Texture(const std::string& path) : mId(0), mPath(path)
{
    LoadTexture();
}

Texture::~Texture()
{
    glDeleteTextures(1, &mId);
}

void Texture::LoadTexture()
{
    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(mPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        LOG_INFO("Texture::Loaded %s", mPath.c_str());
    }
    else
    {
        LOG_ERROR("Texture::Failed %s", mPath.c_str());
    }
    stbi_image_free(data);
}

Texture* Texture::Load(const std::string& path)
{
    auto it = mCache.find(path);
    if (it != mCache.end())
    {
        return it->second;
    }
    else
    {
        Texture* texture = new Texture(path);
        mCache[path] = texture;
        return texture;
    }
}

void Texture::Unload(const std::string& path)
{
    auto it = mCache.find(path);
    if (it != mCache.end())
    {
        delete it->second;
        mCache.erase(it);
    }
    else
    {
        LOG_ERROR("Texture not found in cache: ", path);
    }
}