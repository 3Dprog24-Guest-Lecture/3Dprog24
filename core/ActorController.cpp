#include "ActorController.h"
#include <Actor.h>
#include <GLFW/glfw3.h>

void ActorController::Update(float dt)
{
    UpdateActor(dt);
}

void ActorController::HandleMouseMove(Window* window, double xpos, double ypos)
{}

void ActorController::HandleMouseScroll(Window* window, double xoffset, double yoffset)
{}

void ActorController::HandleMouseButton(Window* window, int button, int action, int mods)
{}

void ActorController::HandleKeyboard(Window* window, int key, int scancode, int action, int mods)
{
    if (!mActor) return;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        mKeyStates[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        mKeyStates[key] = false;
    }
}

void ActorController::UpdateActor(float dt)
{
    if (mKeyStates[GLFW_KEY_D])
    {
        mActor->SetLocalPosition(mActor->GetWorldPosition() + glm::vec3(1.0f, 0.f, 0.f) * dt);
    }
    if (mKeyStates[GLFW_KEY_A])
    {
        mActor->SetLocalPosition(mActor->GetWorldPosition() - glm::vec3(1.0f, 0.f, 0.f) * dt);
    }
    if (mKeyStates[GLFW_KEY_W])
    {
        mActor->SetLocalPosition(mActor->GetWorldPosition() - glm::vec3(0.f, 0.f, 1.0f) * dt);
    }
    if (mKeyStates[GLFW_KEY_S])
    {
        mActor->SetLocalPosition(mActor->GetWorldPosition() + glm::vec3(-0.1f, 0.f, 1.0f) * dt);
    }   
}
