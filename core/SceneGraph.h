#pragma once

#include <Actor.h>

class SceneGraph : public Actor
{
	//BVH mBVH;
public:
	SceneGraph(const std::string& name) : Actor(name) { };
};