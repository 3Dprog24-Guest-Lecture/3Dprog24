#pragma once

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <Collision/Collision.h>
#include <Actor.h>
#include <iostream>
#include <Logger.h>

struct AABB;

/**
 * @brief Interface for objects that can be bounded by an Axis-Aligned Bounding Box (AABB).
 */
class IBounded 
{
public:
    virtual AABB GetAABB() const = 0;
    virtual CollisionProperties GetCollisionProperties() const = 0;
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

    AABB(const glm::vec3& center, const glm::vec3& extent) : center(center), extent(extent) {}

    /**
     * @brief Calculates the intersection between this Axis-Aligned Bounding Box (AABB) and another AABB.
     *
     * This function checks for an intersection between the current AABB object and another AABB provided as a parameter.
     * If an intersection exists, the function returns true. Additionally, if the `mtv` parameter is not a nullptr and there is an intersection,
     * this function calculates and stores the Minimum Translation Vector (MTV) in `mtv`. The MTV represents the smallest vector by which
     * one can move this AABB to resolve the collision with the other AABB.
     *
     * @param other The other AABB with which to check for an intersection.
     * @param mtv A pointer to a glm::vec3 object where the Minimum Translation Vector will be stored if there is an intersection and `mtv` is not nullptr. This parameter is optional.
     * @return bool Returns true if there is an intersection between the two AABBs, false otherwise.
     */
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
                direction[minAxis] = diff[minAxis] > 0.f ? 1.f : -1.f;
                *mtv = direction * minMTV;
            }
        }

        return true; // Intersection exists
    }

    void Expand(const glm::vec3& p)
    {
        auto relativeP = p - center;
        extent = glm::max(abs(relativeP), extent);
    }
};

class AABBActor : public Actor, public IBounded
{
public:
    AABBActor(const std::string& name, AABB aabb = AABB({0.f, 0.f, 0.f}, {0.1f, 0.1f, 0.1f}))
        :Actor(name), mAABB(aabb) {}
   
    virtual AABB GetAABB() const override
    {         
        auto aabb = AABB(mAABB.center ,mAABB.extent);
        aabb.center += GetWorldPosition();
        aabb.extent *= GetWorldScale();
        // AABBs should not really be able to rotate, but this is okay as long as they rotate to a new aligned axis
        aabb.extent = abs(GetWorldRotation() * aabb.extent);
        return aabb;
    }

    virtual CollisionProperties GetCollisionProperties() const
    {
        return mCollisionProperties;
    }

    AABB mAABB{ {0.f, 0.f, 0.f}, {0.5f, 0.5f, 0.5f} };
    CollisionProperties mCollisionProperties{ CollisionType::STATIC, CollisionResponse::BLOCK };
};