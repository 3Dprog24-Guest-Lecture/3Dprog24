#pragma once

#include <Actor.h>
#include <AABB.h>

/**
 * @class SceneGraph
 * @brief Represents the root node of a scene's hierarchical structure.
 *
 * SceneGraph extends the Actor class to act as the root node in a scene's hierarchy,
 * allowing for the organization and management of a collection of actors within a scene.

 * @note This class currently does not implement a bounding volume hierarchy (BVH) for
 * spatial partitioning, which can be used for efficient collision detection and culling.
 */
class SceneGraph : public Actor
{
	//BVH mBVH;
public:
	SceneGraph(const std::string& name) : Actor(name) { };

};