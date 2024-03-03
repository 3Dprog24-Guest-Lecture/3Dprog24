#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Actor.h>

class Camera : public Actor
{
public:
    Camera(const std::string& name,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat rotation = glm::quat(glm::identity<glm::quat>()),
        float aspectRatio = 1280.f / 720.f,
        float nearPlane = 0.1f,
        float farPlane = 100.0f,
        float fieldOfView = 45.0f,
        float maxMovementSeed = 40.f,
        float accelerationSpeed = 20.0f,
        float dampingFactor = 5.0f);

    virtual void Update(float dt) override;

    // Adders
    void AddVelocity(const glm::vec3& velocity);

    // Setters
    void SetAspectRatio(float aspectRatio);
    void SetVelocity(const glm::vec3& velocity);
    void SetAccelerationSpeed(float accelerationSpeed);
    void SetAcceleration(const glm::vec3& acceleration);
    void SetMaxMovementSpeed(float movementSpeed);

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

private:
    void UpdateVelocity(float dt);    
    void UpdatePosition(float dt);
    void UpdateDamping(float dt);
    void UpdateProjectionMatrix();

    float mMaxMovementSpeed;
    float mAccelerationSpeed;
    float mDampingFactor;
    glm::vec3 mVelocity{0.f, 0.f, 0.f};
    glm::vec3 mAcceleration{0.f, 0.f, 0.f};

    float mAspectRatio;
    float mNearPlane;
    float mFarPlane;
    float mFieldOfView;

    glm::mat4 mProjectionMatrix;
};

