#pragma once

#include <Component.h>
#include <glm/glm.hpp>

class PhysicsComponent : public Component
{
	friend class Actor;
private:
	PhysicsComponent(const std::string& name, class Actor* owner)
		:Component(name, owner) {};

	void UpdateForces();
	void UpdateVelocity(float dt);
	void UpdatePosition(float dt);
	void ResetValues(); // Usually a N value in physics is applied to this object when it hits another that slows down acceleration/velocity

	virtual void Update(float dt) override;

	glm::vec3 mVelocity{0.f};
	glm::vec3 mAcceleration{0.f};
};