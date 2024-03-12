#include <Camera.h>

CameraActor::CameraActor(
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
    SetLocalPosition(position);
    SetLocalRotation(rotation);
    UpdateProjectionMatrix();
}

void CameraActor::UpdateVelocity(float dt)
{
    mVelocity += mAcceleration * dt;

    if (glm::length(mVelocity) > mMaxMovementSpeed) 
    {
        mVelocity = glm::normalize(mVelocity) * mMaxMovementSpeed;
    }

    mAcceleration = glm::vec3(0.f);
}

void CameraActor::UpdateDamping(float dt)
{
    float dampingFactor = glm::length(mVelocity) > 0 ? mDampingFactor : 0.0f;

    mVelocity -= mVelocity * dampingFactor * dt;

    if (glm::length(mVelocity) < 0.001f) 
    {
        mVelocity = glm::vec3(0.0f);
    }
}

void CameraActor::UpdateAngularDamping(float dt)
{
    mAngularAcceleration = glm::vec2(0.0f);

    if (glm::length(mAngularVelocity) > 0) 
    {
        mAngularVelocity -= mAngularVelocity * mAngularDampingFactor * dt;
        if (glm::length(mAngularVelocity) < 0.001f) 
        {
            mAngularVelocity = glm::vec2(0.0f);
        }
    }
}

void CameraActor::UpdateAngularVelocity(float dt)
{
    mAngularVelocity += mAngularAcceleration * dt;
    // Clamp angular velocity to the maximum allowed
    mAngularVelocity = glm::clamp(mAngularVelocity, -mMaxAngularSpeed, mMaxAngularSpeed);
}

void CameraActor::UpdateRotation(float dt)
{
    AddYaw(mAngularVelocity.x * dt);
    AddPitch(mAngularVelocity.y * dt);
}

void CameraActor::UpdatePosition(float dt)
{
    glm::vec3 front = GetFront();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::cross(right, front);

    SetLocalPosition(GetLocalPosition() + mVelocity.x * dt * right + mVelocity.y * dt * up + mVelocity.z * dt * front);
}

void CameraActor::Update(float dt)
{
    UpdateVelocity(dt);
    UpdatePosition(dt);
    UpdateDamping(dt);
    UpdateAngularVelocity(dt);
    UpdateRotation(dt);
    UpdateAngularDamping(dt);
}

void CameraActor::AddVelocity(const glm::vec3& velocity)
{
    mVelocity += velocity;
}

void CameraActor::AddYaw(float angleDegrees)
{
    mYaw += angleDegrees;
    // Ensure yaw stays within a reasonable range
    mYaw = std::fmod(mYaw, 360.0f);

    UpdateRotationFromYawPitch();
}

void CameraActor::AddPitch(float angleDegrees)
{
    // Calculate new pitch and clamp it to prevent flipping
    float newPitch = std::clamp(mPitch + angleDegrees, -89.0f, 89.0f);
    mPitch = newPitch;

    UpdateRotationFromYawPitch();
}

void CameraActor::SetAspectRatio(float aspectRatio)
{
    mAspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void CameraActor::SetVelocity(const glm::vec3& velocity)
{
    mVelocity = velocity;
}

void CameraActor::SetAccelerationSpeed(float accelerationSpeed)
{
    mAccelerationSpeed = accelerationSpeed;
}

void CameraActor::SetAcceleration(const glm::vec3& acceleration)
{
    mAcceleration = acceleration;
}

void CameraActor::SetMaxMovementSpeed(float movementSpeed)
{
    mMaxMovementSpeed = movementSpeed;
}

void CameraActor::SetAngularVelocity(const glm::vec2& angularVelocity) {
    mAngularVelocity = angularVelocity;
}

void CameraActor::AddAngularVelocity(const glm::vec2& angularVelocityDelta) 
{
    mAngularVelocity += angularVelocityDelta;
}

const glm::vec2& CameraActor::GetAngularVelocity() const 
{
    return mAngularVelocity;
}

void CameraActor::SetAngularAcceleration(const glm::vec2& angularAcceleration) 
{
    mAngularAcceleration = angularAcceleration;
}

void CameraActor::AddAngularAcceleration(const glm::vec2& angularAccelerationDelta) 
{
    mAngularAcceleration += angularAccelerationDelta;
}

const glm::vec2& CameraActor::GetAngularAcceleration() const 
{
    return mAngularAcceleration;
}

void CameraActor::SetAngularDampingFactor(float dampingFactor) 
{
    mAngularDampingFactor = dampingFactor;
}

float CameraActor::GetAngularDampingFactor() const 
{
    return mAngularDampingFactor;
}

glm::mat4 CameraActor::GetViewMatrix() const
{
    return glm::lookAt(GetLocalPosition(), GetLocalPosition() + GetFront(), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& CameraActor::GetProjectionMatrix() const
{
    return mProjectionMatrix;
}

glm::vec3 CameraActor::GetFront() const
{
    return glm::rotate(GetLocalRotation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 CameraActor::GetUp() const
{
    return glm::rotate(GetLocalRotation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::vec3& CameraActor::GetVelocity() const
{
    return mVelocity;
}

float CameraActor::GetMaxMovementSpeed() const { return mMaxMovementSpeed; }
float CameraActor::GetAspectRatio() const { return mAspectRatio; }
float CameraActor::GetNearPlane() const { return mNearPlane; }
float CameraActor::GetFarPlane() const { return mFarPlane; }
float CameraActor::GetFieldOfView() const { return mFieldOfView; }
float CameraActor::GetAccelerationSpeed() const{ return mAccelerationSpeed; }
const glm::vec3& CameraActor::GetAcceleration() const{ return mAcceleration; }

glm::mat4 CameraActor::GetVPMatrix() const
{
    return mProjectionMatrix * GetViewMatrix();
}

float CameraActor::GetAngularAccelerationSpeed() const
{
    return mAngularAccelerationSpeed;
}

void CameraActor::UpdateProjectionMatrix()
{
    mProjectionMatrix = glm::perspectiveRH(glm::radians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
}

void CameraActor::UpdateRotationFromYawPitch()
{
    glm::quat pitchQuat = glm::angleAxis(glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yawQuat = glm::angleAxis(glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

    // Combined rotation
    glm::quat newRotation = yawQuat * pitchQuat;
    newRotation = glm::normalize(newRotation); // Ensure the quaternion is normalized

    // Assuming SetRotation directly sets the Transform's rotation
    this->SetLocalRotation(newRotation);
}
