#include <PhysicsComponent.h>
#include <Actor.h>

void PhysicsComponent::UpdateForces()
{
	// Fake it for now
	glm::vec3 gravity{0.f, -9.81f * 0.4f, 0.f};
	mAcceleration += gravity;
}

void PhysicsComponent::UpdateVelocity(float dt)
{
	mVelocity += mAcceleration * dt;
}

void PhysicsComponent::UpdatePosition(float dt)
{
	mOwner->SetWorldPosition(mOwner->GetWorldPosition() + (mAcceleration * dt));
}

void PhysicsComponent::ResetValues()
{
	mAcceleration = glm::vec3(0.f);
	mVelocity = glm::vec3(0.f);
}

void PhysicsComponent::Update(float dt)
{
	UpdateForces();
	UpdateVelocity(dt);
	UpdatePosition(dt);
	ResetValues();
}
