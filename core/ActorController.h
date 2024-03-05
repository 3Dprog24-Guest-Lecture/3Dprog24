#pragma once

#include <Controller.h>
#include <map>

/**
 * @class ActorController
 * @brief Controls actor movement based on user input.
 *
 * ActorController is responsible for handling user inputs. It is designed to work with a
 * Actor to update its transformation based on the input.
 */
class ActorController : public IController
{
public:
    ActorController(class Actor* actor) : mActor(actor) {}

    virtual void Update(float dt) override;
    virtual void HandleMouseMove(class Window* window, double xpos, double ypos) override;
    virtual void HandleMouseScroll(class Window* window, double xoffset, double yoffset) override;
    virtual void HandleMouseButton(class Window* window, int button, int action, int mods) override;
    virtual void HandleKeyboard(class Window* window, int key, int scancode, int action, int mods) override;

private:
    void UpdateActor(float dt);

    class Actor* mActor;
    std::map<int, bool> mKeyStates;
};