#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Actor.h>

/**
 * @class CameraActor
 * @brief Represents a camera in the scene.
 *
 * CameraActor inherits from Actor, allowing it to be part of the scene graph.
 */
class CameraActor : public Actor
{
public:
    CameraActor(const std::string& name,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat rotation = glm::quat(glm::identity<glm::quat>()),
        float aspectRatio = 1280.f / 720.f,
        float nearPlane = 0.1f,
        float farPlane = 100.0f,
        float fieldOfView = 45.0f,
        float maxMovementSeed = 100.f,
        float accelerationSpeed = 50.0f,
        float dampingFactor = 5.0f);

    virtual void Update(float dt) override;

    // Adders
    void AddVelocity(const glm::vec3& velocity);
    // Add or update methods to modify yaw and pitch
    void AddYaw(float angleDegrees) 
    {
        mYaw += angleDegrees;
        // Ensure yaw stays within a reasonable range
        mYaw = std::fmod(mYaw, 360.0f);

        UpdateRotationFromYawPitch();
    }

    void AddPitch(float angleDegrees) 
    {
        // Calculate new pitch and clamp it to prevent flipping
        float newPitch = std::clamp(mPitch + angleDegrees, -89.0f, 89.0f);
        mPitch = newPitch;

        UpdateRotationFromYawPitch();
    }
    // Setters
    void SetAspectRatio(float aspectRatio);
    void SetVelocity(const glm::vec3& velocity);
    void SetAccelerationSpeed(float accelerationSpeed);
    void SetAcceleration(const glm::vec3& acceleration);
    void SetMaxMovementSpeed(float movementSpeed);

    void SetAngularVelocity(const glm::vec2& angularVelocity);
    void AddAngularVelocity(const glm::vec2& angularVelocityDelta);
    const glm::vec2& GetAngularVelocity() const;

    void SetAngularAcceleration(const glm::vec2& angularAcceleration);
    void AddAngularAcceleration(const glm::vec2& angularAccelerationDelta);
    const glm::vec2& GetAngularAcceleration() const;

    void SetAngularDampingFactor(float dampingFactor);
    float GetAngularDampingFactor() const;

    // Getters
    float GetMaxMovementSpeed() const;
    float GetAspectRatio() const;
    float GetNearPlane() const;
    float GetFarPlane() const;
    float GetFieldOfView() const;
    float GetAccelerationSpeed() const;
    const glm::vec3& GetAcceleration() const;

    glm::vec3 GetFront() const;
    glm::vec3 GetUp() const;
    const glm::vec3& GetVelocity() const;
    glm::mat4 GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    glm::mat4 GetVPMatrix() const;
    float GetAngularAccelerationSpeed() const;

 private:
    void UpdateVelocity(float dt);    
    void UpdatePosition(float dt);
    void UpdateRotation(float dt);
    void UpdateAngularVelocity(float dt);
    void UpdateAngularDamping(float dt);
    void UpdateDamping(float dt);
    void UpdateProjectionMatrix();

    void UpdateRotationFromYawPitch() 
    {
        glm::quat pitchQuat = glm::angleAxis(glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat yawQuat = glm::angleAxis(glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

        // Combined rotation
        glm::quat newRotation = yawQuat * pitchQuat;
        newRotation = glm::normalize(newRotation); // Ensure the quaternion is normalized

        // Assuming SetRotation directly sets the Transform's rotation
        this->SetRotation(newRotation);
    }

    float mMaxMovementSpeed;
    float mAccelerationSpeed;
    float mDampingFactor;

    float mAngularAccelerationSpeed = 1000.f;
    float mMaxAngularSpeed = 500.f;
    float mAngularDampingFactor = 20.0f;

    glm::vec3 mVelocity{0.f, 0.f, 0.f};
    glm::vec3 mAcceleration{0.f, 0.f, 0.f};
    glm::vec2 mAngularVelocity{0.f, 0.f}; // x for yaw, y for pitch
    glm::vec2 mAngularAcceleration{0.f, 0.f};

    float mAspectRatio;
    float mNearPlane;
    float mFarPlane;
    float mFieldOfView;

    float mYaw = 0.0f; // Yaw angle in degrees
    float mPitch = 0.0f; // Pitch angle in degrees

    glm::mat4 mProjectionMatrix;
};

