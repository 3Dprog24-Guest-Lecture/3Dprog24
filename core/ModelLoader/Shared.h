#pragma once
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

inline std::string ToRelativePath(const std::string& basePath, const std::string& absolutePath) {
    fs::path base(basePath);
    fs::path absolute(absolutePath);
    fs::path relative = fs::relative(absolute, base);
    return relative.string();
}

inline std::string GetFileNameFromPath(const std::string& filePath)
{
    size_t pos = filePath.find_last_of("\\/");
    if (pos != std::string::npos)
    {
        return filePath.substr(pos + 1);
    }
    return filePath;
}

inline std::string GetDirectoryPath(const std::string& filePath)
{
    size_t pos = filePath.find_last_of("\\/");
    if (pos != std::string::npos)
    {
        return filePath.substr(0, pos + 1);
    }
    return "";
}

inline std::string GetFileExtension(const std::string& filePath) {
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos)
    {
        return filePath.substr(pos);
    }
    return "";
}

inline std::string RemoveFileExtension(const std::string& filePath)
{
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos)
    {
        return filePath.substr(0, pos);
    }
    return filePath;
}

inline bool HasCollisionPrefix(const std::string& filePath, std::string& outPrefix)
{
    if (filePath[0] != '_')
        return false;

    size_t pos = std::string(filePath.begin() + 1, filePath.end()).find_first_of('_');
    if (pos == std::string::npos)
        return false;

    if (std::string(filePath.begin(), filePath.begin() + pos + 2) != "_Collision_")
        return false;

    outPrefix = std::string(filePath.begin() + pos + 2, filePath.end());

    return true;
}
