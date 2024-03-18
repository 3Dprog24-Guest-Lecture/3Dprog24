#pragma once

#include <string>
#include <unordered_set>
#include <stdexcept>

/**
 * @struct TagUnique
 * @brief Ensures uniqueness of tags within the application.
 *
 * The TagUnique struct is designed to manage tag uniqueness across the application,
 * preventing duplicate tags from being created. It uses a static unordered set to keep
 * track of all existing tags. Attempting to create a TagUnique with a value that already
 * exists in the set will result in a runtime_error.
 */
struct TagUnique
{
    TagUnique(const std::string& val)
    {
        if (msExistingTags.find(val) != msExistingTags.end())
        {
            throw std::runtime_error("Tag already exists: " + val);
        }
        mValue = val;
        msExistingTags.insert(val);
    }

    TagUnique(const TagUnique&) = delete;
    TagUnique& operator=(const TagUnique&) = delete;

    TagUnique(TagUnique&&) = default;
    TagUnique& operator=(TagUnique&&) = default;

    ~TagUnique()
    {
        msExistingTags.erase(mValue);
    }

    const std::string& GetValue() const
    {
        return mValue;
    }

    static bool TagExists(const std::string& tag)
    {
        return msExistingTags.find(tag) != msExistingTags.end();
    }

    static void ClearCache()
    {
        msExistingTags.clear();
    }

private:
    static std::unordered_set<std::string> msExistingTags;
    std::string mValue;
};

inline std::unordered_set<std::string> TagUnique::msExistingTags{};
