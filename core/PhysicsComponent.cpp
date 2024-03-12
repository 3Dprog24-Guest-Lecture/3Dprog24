#include <PhysicsComponent.h>
#include <Actor.h>

void PhysicsComponent::UpdateForces()
{
	// Usually this should be around 9.81, but that is slow in games
	glm::vec3 gravity{0.f, -18.f, 0.f};
	mAcceleration = gravity;
}

void PhysicsComponent::UpdateVelocity(float dt)
{
	mVelocity += mAcceleration * dt;
}

void PhysicsComponent::UpdatePosition(float dt)
{
	mOwner->SetWorldPosition(mOwner->GetWorldPosition() + (glm::vec3(0.f, -1.f, 0.f) * dt));
}

void PhysicsComponent::ResetValues()
{
	mVelocity = glm::vec3(0.f);
}

void PhysicsComponent::Update(float dt)
{
	UpdateForces();
	UpdateVelocity(dt);
	UpdatePosition(dt);
	ResetValues();
}
