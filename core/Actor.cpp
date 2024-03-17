#include "Actor.h"

Actor::~Actor()
{
    for (auto component : mComponents)
    {
        delete component;
    }
}

void Actor::UpdateComponents(float dt)
{
    for (auto component : mComponents)
    {
        component->Update(dt);
    };
}

void Actor::SetParent(Actor* parent)
{
    if (mParent) {
        mParent->RemoveChild(this);
    }

    mParent = parent;

    if (mParent) 
    {
        mParent->AddChild(this);
    }
}

void Actor::AddChild(Actor* child)
{
    if (child) 
    {
        mChildren.push_back(child);
        child->mParent = this;
    }
}

void Actor::RemoveChild(Actor* child)
{
    auto it = std::find(mChildren.begin(), mChildren.end(), child);
    if (it != mChildren.end()) 
    {
        mChildren.erase(it);
        if (child->mParent == this) 
        {
            child->mParent = nullptr;
        }
    }
}

void Actor::SetTransform(const Transform& transform)
{
    mTransform = transform;
}

void Actor::SetLocalTransformMatrix(const glm::mat4& transformMatrix)
{
    mTransform.SetTransformMatrix(transformMatrix);
}

void Actor::SetLocalPosition(const glm::vec3& position)
{
    mTransform.SetPosition(position);
}

void Actor::SetWorldPosition(const glm::vec3& position)
{
    if (mParent)
    {
        glm::mat4 parentGlobalInverse = glm::inverse(mParent->GetWorldTransformMatrix());
        glm::vec4 localPosition = parentGlobalInverse * glm::vec4(position, 1.0f);
        mTransform.SetPosition(glm::vec3(localPosition));
    }
    else {
        mTransform.SetPosition(position);
    }
}

void Actor::SetLocalRotation(const glm::quat& rotation)
{
    mTransform.SetRotation(rotation);
}

void Actor::SetWorldRotation(const glm::quat& rotation)
{
    if (mParent)
    {
        glm::quat parentGlobalRotationInverse = glm::inverse(glm::quat_cast(mParent->GetWorldTransformMatrix()));
        glm::quat localRotation = parentGlobalRotationInverse * rotation;
        mTransform.SetRotation(localRotation);
    }
    else
    {
        mTransform.SetRotation(rotation);
    }
}

void Actor::SetLocalScale(const glm::vec3& scale)
{
    mTransform.SetScale(scale);
}

void Actor::SetWorldScale(const glm::vec3& scale)
{
    if (mParent)
    {
        glm::vec3 parentGlobalScale = mParent->GetWorldScale();
        glm::vec3 relativeScale = scale / parentGlobalScale;
        mTransform.SetScale(relativeScale);
    }
    else
    {
        mTransform.SetScale(scale);
    }
}

const Transform& Actor::GetLocalTransform() const
{
    return mTransform;
}

const glm::mat4 Actor::GetLocalTransformMatrix() const
{
    return mTransform.GetTransformMatrix();
}

glm::mat4 Actor::GetWorldTransformMatrix() const
{
    glm::mat4 globalTransform = mTransform.GetTransformMatrix();
    const Actor* current = this;
    while (current->mParent != nullptr)
    {
        globalTransform = current->mParent->GetLocalTransformMatrix() * globalTransform;
        current = current->mParent;
    }
    return globalTransform;
}

std::vector<Actor*>& Actor::GetChildren()
{
    return mChildren;
}

glm::vec3 Actor::GetRight() const
{
    return mTransform.GetRight();
}

const glm::vec3& Actor::GetLocalPosition() const
{
    return mTransform.GetPosition();
}

glm::vec3 Actor::GetWorldPosition() const
{
    glm::vec3 globalPosition = mTransform.GetPosition();
    const Actor* current = this->mParent;
    while (current != nullptr)
    {
        globalPosition = glm::rotate(current->mTransform.GetRotation(), globalPosition);
        globalPosition += current->mTransform.GetPosition();
        current = current->mParent;
    }
    return globalPosition;
}

const glm::quat& Actor::GetLocalRotation() const
{
    return mTransform.GetRotation();
}

glm::quat Actor::GetWorldRotation() const 
{
    glm::quat globalRotation = mTransform.GetRotation();
    const Actor* current = this->mParent;
    while (current != nullptr) 
    {
        globalRotation = current->mTransform.GetRotation() * globalRotation;
        current = current->mParent;
    }
    return globalRotation;
}

const glm::vec3& Actor::GetLocalScale() const
{
    return mTransform.GetScale();
}

glm::vec3 Actor::GetWorldScale() const
{
    glm::vec3 globalScale = mTransform.GetScale();
    const Actor* current = this->mParent;
    while (current != nullptr) 
    {
        globalScale *= current->mTransform.GetScale();
        current = current->mParent;
    }
    return globalScale;
}
