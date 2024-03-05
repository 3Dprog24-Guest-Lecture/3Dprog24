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
    void AddYaw(float angleDegrees);
    void AddPitch(float angleDegrees);
    void AddAngularVelocity(const glm::vec2& angularVelocityDelta);
    void AddAngularAcceleration(const glm::vec2& angularAccelerationDelta);

    // Setters
    void SetAspectRatio(float aspectRatio);
    void SetVelocity(const glm::vec3& velocity);
    void SetAccelerationSpeed(float accelerationSpeed);
    void SetAcceleration(const glm::vec3& acceleration);
    void SetMaxMovementSpeed(float movementSpeed);
    void SetAngularVelocity(const glm::vec2& angularVelocity);
    void SetAngularAcceleration(const glm::vec2& angularAcceleration);   
    void SetAngularDampingFactor(float dampingFactor);
    
    // Getters
    float GetMaxMovementSpeed() const;
    float GetAspectRatio() const;
    float GetNearPlane() const;
    float GetFarPlane() const;
    float GetFieldOfView() const;
    float GetAccelerationSpeed() const;
    const glm::vec3& GetAcceleration() const;
    const glm::vec2& GetAngularVelocity() const;
    const glm::vec2& GetAngularAcceleration() const;
    float GetAngularDampingFactor() const;
    glm::vec3 GetFront() const;
    glm::vec3 GetUp() const;
    const glm::vec3& GetVelocity() const;
    glm::mat4 GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    glm::mat4 GetVPMatrix() const;
    float GetAngularAccelerationSpeed() const;

 private:
    // Update
    void UpdateVelocity(float dt);    
    void UpdatePosition(float dt);
    void UpdateRotation(float dt);
    void UpdateAngularVelocity(float dt);
    void UpdateAngularDamping(float dt);
    void UpdateDamping(float dt);
    void UpdateProjectionMatrix();
    void UpdateRotationFromYawPitch();

    // Speed
    float mMaxMovementSpeed;
    float mAccelerationSpeed;
    float mAngularAccelerationSpeed{ 1000.f };
    float mMaxAngularSpeed{ 500.f };

    // Damping
    float mDampingFactor;
    float mAngularDampingFactor{ 20.0f };
    
    // Velocity and acceleration
    glm::vec3 mVelocity{0.f, 0.f, 0.f};
    glm::vec3 mAcceleration{0.f, 0.f, 0.f};
    glm::vec2 mAngularVelocity{0.f, 0.f}; // x for yaw, y for pitch
    glm::vec2 mAngularAcceleration{0.f, 0.f};

    // Perspecitve
    float mAspectRatio;
    float mNearPlane;
    float mFarPlane;
    float mFieldOfView;

    // Todo: Update these values on other rotation operations
    float mYaw = 0.0f; // Yaw angle in degrees
    float mPitch = 0.0f; // Pitch angle in degrees

    glm::mat4 mProjectionMatrix;
};

