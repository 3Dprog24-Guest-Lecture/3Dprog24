#include "Actor.h"

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

void Actor::SetPosition(const glm::vec3& position, Actor::TransformSpace type)
{
    if (type == TransformSpace::Local)
    {
        mTransform.SetPosition(position);
    }
    else if (type == TransformSpace::Global)
    {
        if (mParent) 
        {
            glm::mat4 parentGlobalInverse = glm::inverse(mParent->GetGlobalTransformMatrix());
            glm::vec4 localPosition = parentGlobalInverse * glm::vec4(position, 1.0f);
            mTransform.SetPosition(glm::vec3(localPosition));
        }
        else {
            mTransform.SetPosition(position);
        }
    }
}

void Actor::SetRotation(const glm::quat& rotation, Actor::TransformSpace type)
{
    if (type == TransformSpace::Local) 
    {
        mTransform.SetRotation(rotation);
    }
    else if (type == TransformSpace::Global) 
    {
        if (mParent) 
        {
            glm::quat parentGlobalRotationInverse = glm::inverse(glm::quat_cast(mParent->GetGlobalTransformMatrix()));
            glm::quat localRotation = parentGlobalRotationInverse * rotation;
            mTransform.SetRotation(localRotation);
        }
        else 
        {
            mTransform.SetRotation(rotation);
        }
    }
}

void Actor::SetScale(const glm::vec3& scale, Actor::TransformSpace type)
{
    if (type == TransformSpace::Local) 
    {
        mTransform.SetScale(scale);
    }
    else if (type == TransformSpace::Global) 
    {
        if (mParent) 
        {
            glm::vec3 parentGlobalScale = mParent->GetGlobalScale();
            glm::vec3 relativeScale = scale / parentGlobalScale;
            mTransform.SetScale(relativeScale);
        }
        else 
        {
            mTransform.SetScale(scale);
        }
    }
}

const Transform& Actor::GetTransform() const
{
    return mTransform;
}

const glm::mat4 Actor::GetTransformMatrix() const
{
    return mTransform.GetTransformMatrix();
}

glm::mat4 Actor::GetGlobalTransformMatrix() const
{
    glm::mat4 globalTransform = mTransform.GetTransformMatrix();
    const Actor* current = this;
    while (current->mParent != nullptr)
    {
        globalTransform = current->mParent->GetTransformMatrix() * globalTransform;
        current = current->mParent;
    }
    return globalTransform;
}

std::vector<Actor*>& Actor::GetChildren()
{
    return mChildren;
}

const glm::vec3& Actor::GetPosition() const
{
    return mTransform.GetPosition();
}

glm::vec3 Actor::GetGlobalPosition() const
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

const glm::quat& Actor::GetRotation() const
{
    return mTransform.GetRotation();
}

glm::quat Actor::GetGlobalRotation() const 
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

const glm::vec3& Actor::GetScale() const
{
    return mTransform.GetScale();
}

glm::vec3 Actor::GetGlobalScale() const
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
