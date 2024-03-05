#pragma once

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <memory>

struct AABB;

/**
 * @brief Interface for objects that can be bounded by an Axis-Aligned Bounding Box (AABB).
 */
class IBounded 
{
public:
    virtual AABB GetAABB() = 0;
};

/**
 * @brief Represents an Axis-Aligned Bounding Box (AABB) used for collision detection.
 *
 * The AABB is defined by a center point and an extent (half-size in each dimension),
 * allowing for efficient intersection tests and spatial partitioning.
 */
struct AABB 
{
    glm::vec3 center{0.f, 0.f ,0.f};
    glm::vec3 extent{0.5f, 0.5f, 0.5f};

    AABB() = default;
    AABB(const glm::vec3& center, const glm::vec3& extent) : center(center), extent(extent) {}

    // Calculate intersection between this AABB and another
    // Returns true if there's an intersection, false otherwise.
    // If mtv is not nullptr and there's an intersection, it will store the Minimum Translation Vector to resolve the collision.
    bool Intersect(const AABB& other, glm::vec3* mtv) const 
    {
        glm::vec3 diff = other.center - this->center;
        glm::vec3 sumExtent = this->extent + other.extent;

        // For each axis, check if there's overlap
        for (int i = 0; i < 3; ++i) 
        {
            if (abs(diff[i]) >= sumExtent[i])
            {
                return false; // No intersection on this axis
            }
        }

        if (mtv) 
        {
            // Calculate MTV
            glm::vec3 mtvValues;
            for (int i = 0; i < 3; ++i) 
            {
                mtvValues[i] = sumExtent[i] - abs(diff[i]);
            }

            // Find the minimum non-zero translation vector
            float minMTV = FLT_MAX;
            int minAxis = -1;
            for (int i = 0; i < 3; ++i) 
            {
                if (mtvValues[i] < minMTV && mtvValues[i] > 0)
                {
                    minMTV = mtvValues[i];
                    minAxis = i;
                }
            }

            if (minAxis != -1) 
            {
                glm::vec3 direction = glm::vec3(0);
                direction[minAxis] = diff[minAxis] > 0 ? 1 : -1;
                *mtv = direction * minMTV;
            }
        }

        return true; // Intersection exists
    }
};