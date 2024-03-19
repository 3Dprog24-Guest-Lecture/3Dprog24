#pragma once

#include <Component.h>
#include <glm/glm.hpp>

/**
 * @brief Represents the physics component of an actor.
 *
 * This component is responsible for handling the physics-related attributes and behaviors
 * of an actor, such as velocity and acceleration. It updates the actor transform based on these properties.
 */
class PhysicsComponent : public Component
{
	friend class Actor;

private:
	/**
	 * @brief Constructs a PhysicsComponent attached to an actor.
	 *
	 * @param name The name of the component.
	 * @param owner Pointer to the actor that owns this component.
	 */
	PhysicsComponent(const std::string& name, class Actor* owner)
		: Component(name, owner) {};

	/**
	 * @brief Updates forces affecting the component.
	 */
	void UpdateForces();

	/**
	 * @brief Updates the velocity of the component based on acceleration and delta time.
	 *
	 * @param dt Time elapsed since the last update.
	 */
	void UpdateVelocity(float dt);

	/**
	 * @brief Updates the position of the component based on velocity and delta time.
	 *
	 * @param dt Time elapsed since the last update.
	 */
	void UpdatePosition(float dt);

	/**
	 * @brief Resets physics values. This is typically calculated with forces when an object collides
	 * with another, affecting its acceleration or velocity. This is just a simplified version.
	 */
	void ResetValues();

	/**
	 * @brief Updates the component, overriding the Component class's update method.
	 *
	 * @param dt Time elapsed since the last update.
	 */
	virtual void Update(float dt) override;

	glm::vec3 mVelocity{0.f};
	glm::vec3 mAcceleration{0.f};
	float mMaxSpeed = 4.f;
};
