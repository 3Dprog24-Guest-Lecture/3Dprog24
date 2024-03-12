#pragma once
#include <Tag.h>
#include <Transform.h>

class Component 
{
    // Friend classes should technically be avoided because they break encapsulation
    // But will be used in this case cause we only want an actor to create, init and update components
    // For all derived classes add actor as a friend
    friend class Actor;
public:
    virtual ~Component() {}

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    Component(Component&&) = delete;
    Component& operator=(Component&&) = delete;
  
protected:
    Component(const std::string& name, class Actor* owner)
        :mTag(name), mOwner(owner)
    {}

    // Init runs when the component is added to an actor
    virtual void Init() {}
    // Runs on actor update
    virtual void Update(float dt) {}

protected:
    class Actor* mOwner;
private:
    TagUnique mTag;
};