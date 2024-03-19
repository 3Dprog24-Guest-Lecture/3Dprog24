#include <Physics/PhysicsComponent.h>
#include <Actor.h>

void PhysicsComponent::UpdateForces()
{
    glm::vec3 gravity{0.f, -9.81f, 0.f};
    mAcceleration = gravity;
}

void PhysicsComponent::UpdateVelocity(float dt)
{
    mVelocity += mAcceleration * dt;

    if (glm::length(mVelocity) > mMaxSpeed)
    {
        mVelocity = glm::normalize(mVelocity) * mMaxSpeed;
    }
}

void PhysicsComponent::UpdatePosition(float dt)
{
    mOwner->SetWorldPosition(mOwner->GetWorldPosition() + (mVelocity * dt));
}

void PhysicsComponent::ResetValues()
{
    mAcceleration = glm::vec3(0.f);
}

void PhysicsComponent::Update(float dt)
{
    UpdateForces();
    UpdateVelocity(dt);
    UpdatePosition(dt);
    ResetValues();
}
