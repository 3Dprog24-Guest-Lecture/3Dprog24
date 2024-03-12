#pragma once

#include <Lights/Lights.h>
#include <glm/glm.hpp>
#include <Actor.h>

class DirectionalLight : public Light
{
public:
	virtual glm::vec3 GetDirection() const = 0;
};

class DirectionalLightActor : public DirectionalLight, public Actor
{
public:
	DirectionalLightActor(const std::string& name)
		: Actor(name) {};

	virtual glm::vec3 GetDirection() const override
	{
		return GetLocalRotation() * glm::vec3(0.f, 0.f, -1.f);
	}
};