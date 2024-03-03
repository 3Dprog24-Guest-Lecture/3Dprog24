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
private:
    static std::unordered_set<std::string> mExistingTags;
    std::string mValue;

public:
    TagUnique(const std::string& val)
    {
        if (mExistingTags.find(val) != mExistingTags.end())
        {
            throw std::runtime_error("Tag already exists: " + val);
        }
        mValue = val;
        mExistingTags.insert(val);
    }

    TagUnique(const TagUnique&) = delete;
    TagUnique& operator=(const TagUnique&) = delete;

    TagUnique(TagUnique&&) = default;
    TagUnique& operator=(TagUnique&&) = default;

    ~TagUnique()
    {
        mExistingTags.erase(mValue);
    }

    const std::string& GetValue() const
    {
        return mValue;
    }

    static bool TagExists(const std::string& tag)
    {
        return mExistingTags.find(tag) != mExistingTags.end();
    }

    static void ClearCache()
    {
        mExistingTags.clear();
    }
};

inline std::unordered_set<std::string> TagUnique::mExistingTags{};
