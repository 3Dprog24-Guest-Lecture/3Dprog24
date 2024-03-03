#pragma once

#include <vector>
#include <Transform.h>
#include <Tag.h>

class Actor
{
public:
    enum class TransformSpace 
    {
        Local,
        Global
    };

    Actor(const std::string& name) :mTag(name), mParent(nullptr) {};

	Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;

	Actor(Actor&&) = default;
    Actor& operator=(Actor&&) = default;

	~Actor() = default;

    virtual void Update(float dt) {};

    void SetParent(Actor* parent);
    void AddChild(Actor* child);
    void RemoveChild(Actor* child);

    // Setters
    void SetTransform(const Transform& transform);
    void SetPosition(const glm::vec3& position, Actor::TransformSpace type = Actor::TransformSpace::Local);
    void SetRotation(const glm::quat& rotation, Actor::TransformSpace type = Actor::TransformSpace::Local);
    void SetScale(const glm::vec3& scale, Actor::TransformSpace type = Actor::TransformSpace::Local);

    // Getters
    const glm::vec3& GetPosition() const;
    glm::vec3 GetGlobalPosition() const;
    const glm::quat& GetRotation() const;
    glm::quat GetGlobalRotation() const;
    const glm::vec3& GetScale() const;
    glm::vec3 GetGlobalScale() const;
    const glm::mat4 GetTransformMatrix() const;
    glm::mat4 GetGlobalTransformMatrix() const;
    const Transform& GetTransform() const;
    std::vector<Actor*>& GetChildren();
protected:
    Actor* mParent{nullptr};
    std::vector<Actor*> mChildren;
private:
    TagUnique mTag;
    Transform mTransform{};
};