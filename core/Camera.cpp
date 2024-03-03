#include <Camera.h>

Camera::Camera(
    const std::string& name,
    glm::vec3 position,
    glm::quat rotation,
    float aspectRatio,
    float nearPlane,
    float farPlane,
    float fieldOfView,
    float maxMovementSpeed,
    float accelerationSpeed,
    float dampingFactor)
    : Actor(name),
    mMaxMovementSpeed(maxMovementSpeed),
    mAccelerationSpeed(accelerationSpeed),
    mDampingFactor(dampingFactor),
    mAspectRatio(aspectRatio),
    mNearPlane(nearPlane),
    mFarPlane(farPlane),
    mFieldOfView(fieldOfView)
{
    SetPosition(position);
    SetRotation(rotation);
    UpdateProjectionMatrix();
}

void Camera::UpdateVelocity(float dt)
{
    mVelocity += mAcceleration * dt;

    if (glm::length(mVelocity) > mMaxMovementSpeed) 
    {
        mVelocity = glm::normalize(mVelocity) * mMaxMovementSpeed;
    }
}

void Camera::UpdateDamping(float dt)
{
    float dampingFactor = glm::length(mVelocity) > 0 ? mDampingFactor : 0.0f;

    mVelocity -= mVelocity * dampingFactor * dt;

    if (glm::length(mVelocity) < 0.001f) 
    {
        mVelocity = glm::vec3(0.0f);
    }
}

void Camera::UpdatePosition(float dt)
{
    glm::vec3 front = GetFront();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::cross(right, front);

    SetPosition(GetPosition() + mVelocity.x * dt * right + mVelocity.y * dt * up + mVelocity.z * dt * front);
}

void Camera::Update(float dt)
{
    UpdateVelocity(dt);
    UpdatePosition(dt);
    UpdateDamping(dt);
}

void Camera::AddVelocity(const glm::vec3& velocity)
{
    mVelocity += velocity;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    mAspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void Camera::SetVelocity(const glm::vec3& velocity)
{
    mVelocity = velocity;
}

void Camera::SetAccelerationSpeed(float accelerationSpeed)
{
    mAccelerationSpeed = accelerationSpeed;
}

void Camera::SetAcceleration(const glm::vec3& acceleration)
{
    mAcceleration = acceleration;
}

void Camera::SetMaxMovementSpeed(float movementSpeed)
{
    mMaxMovementSpeed = movementSpeed;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(GetPosition(), GetPosition() + GetFront(), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return mProjectionMatrix;
}

glm::vec3 Camera::GetFront() const
{
    return glm::rotate(GetRotation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Camera::GetUp() const
{
    return glm::rotate(GetRotation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::vec3& Camera::GetVelocity() const
{
    return mVelocity;
}

float Camera::GetMaxMovementSpeed() const { return mMaxMovementSpeed; }
float Camera::GetAspectRatio() const { return mAspectRatio; }
float Camera::GetNearPlane() const { return mNearPlane; }
float Camera::GetFarPlane() const { return mFarPlane; }
float Camera::GetFieldOfView() const { return mFieldOfView; }
float Camera::GetAccelerationSpeed() const{ return mAccelerationSpeed; }
const glm::vec3& Camera::GetAcceleration() const{ return mAcceleration; }

glm::mat4 Camera::GetVPMatrix() const
{
    return mProjectionMatrix * GetViewMatrix();
}

void Camera::UpdateProjectionMatrix()
{
    mProjectionMatrix = glm::perspective(glm::radians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
}