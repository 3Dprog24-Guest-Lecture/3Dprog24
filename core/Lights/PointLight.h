#pragma once
#include <Lights/Lights.h>
#include <Actor.h>

// Change this variable in the shader too
#define MAX_POINT_LIGHTS 64

/**
 * @brief Internal class for a point light source in a 3D scene.
 */
class PointLight : public Light
{
public:
	virtual glm::vec3 GetPosition() const = 0;

	float constant = 1.f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

/**
 * @brief A concrete actor class representing a point light source in a 3D scene.
 *
 * Inherits from both Actor and PointLight to implement a point light that can be positioned
 * and moved within the scene. It provides the implementation of GetPosition to return the
 * global position of the light actor.
 */
class PointLightActor : public Actor, public PointLight
{
public:
	PointLightActor(const std::string& name) :Actor(name) {};
		
	virtual glm::vec3 GetPosition() const
	{
		return GetWorldPosition();
	}
};