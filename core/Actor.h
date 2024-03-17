#pragma once

#include <vector>
#include <Transform.h>
#include <Tag.h>
#include <Component.h>
#include <type_traits>

class Actor
{
public:
    Actor(const std::string& name) :mTag(name), mParent(nullptr) {};

	Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;

	Actor(Actor&&) = default;
    Actor& operator=(Actor&&) = default;

	~Actor();

    // Actor update should run first
    virtual void Update(float dt) {};
    // Components update after
    void UpdateComponents(float dt);

    // Scene Graph
    void SetParent(Actor* parent);
    void AddChild(Actor* child);

    template <typename T>
    void AddComponent(const std::string& componentName)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

        auto component = new T(componentName, this);
        component->Init();
        mComponents.emplace_back(component);
    }

    void RemoveChild(Actor* child);

    // Setters
    void SetTransform(const Transform& transform);
    void SetLocalTransformMatrix(const glm::mat4& transformMatrix);
    void SetLocalPosition(const glm::vec3& position);
    void SetLocalRotation(const glm::quat& rotation);
    void SetLocalScale(const glm::vec3& scale);
    void SetWorldPosition(const glm::vec3& position);
    void SetWorldRotation(const glm::quat& rotation);
    void SetWorldScale(const glm::vec3& scale);

    // Getters
    const glm::vec3& GetLocalPosition() const;
    glm::vec3 GetWorldPosition() const;
    const glm::quat& GetLocalRotation() const;
    glm::quat GetWorldRotation() const;
    const glm::vec3& GetLocalScale() const;
    glm::vec3 GetWorldScale() const;
    const glm::mat4 GetLocalTransformMatrix() const;
    glm::mat4 GetWorldTransformMatrix() const;
    const Transform& GetLocalTransform() const;
    std::vector<Actor*>& GetChildren();
    glm::vec3 GetRight() const;

    /**
     * @brief Adds actors of type T to a vector if they match the current object type.
     *
     * This function recursively checks the current object and its children for type T. If the object is of type T,
     * it is added to the provided vector of Actor pointers. This allows for collecting all actors of a specific type
     * within a hierarchy.
     *
     * @param actors A reference to a vector of Actor pointers to be filled with actors of type T.
     */
    template <typename T>
    void Query(std::vector<Actor*>& actors)
    {
        if (dynamic_cast<T*>(this))
            actors.emplace_back(this);

        for (auto child : this->mChildren)
            child->Query<T>(actors);
    }
protected:
    // Scene graph
    Actor* mParent{nullptr};
    std::vector<Actor*> mChildren;

    // Components
    std::vector<Component*> mComponents;
private:
    TagUnique mTag;
    Transform mTransform{};
};