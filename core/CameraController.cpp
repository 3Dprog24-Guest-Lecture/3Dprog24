#include "CameraController.h"
#include <Logger.h>
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Window.h>

void CameraController::Update(float dt)
{
    UpdateCameraAcceleration();
}

void CameraController::HandleMouseMove(Window* window, double xpos, double ypos)
{
    if (!mRightMousePressed) return;

    float xoffset = mLastX - static_cast<float>(xpos);
    float yoffset = mLastY - static_cast<float>(ypos);

    mLastX = static_cast<float>(xpos);
    mLastY = static_cast<float>(ypos);

    xoffset *= mMouseSensitivity;
    yoffset *= mMouseSensitivity;

    auto angularSpeed = mCamera->GetAngularAccelerationSpeed();

    mCamera->AddAngularAcceleration({ xoffset * angularSpeed, yoffset * angularSpeed });
}

void CameraController::HandleMouseButton(Window* window, int button, int action, int mods)
{ 
    if (button == GLFW_MOUSE_BUTTON_RIGHT) 
    {
        if (action == GLFW_PRESS) 
        {
            mRightMousePressed = true;
            mLastX = static_cast<float>(window->GetWidth() / 2);
            mLastY = static_cast<float>(window->GetHeight() / 2);
            glfwSetCursorPos(window->GetGLFWWindow(), window->GetWidth() / 2, window->GetHeight() / 2);
            glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE) 
        {
            mRightMousePressed = false;       
            glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mLastX = static_cast<float>(window->GetWidth() / 2);
            mLastY = static_cast<float>(window->GetHeight() / 2);
            glfwSetCursorPos(window->GetGLFWWindow(), window->GetWidth() / 2, window->GetHeight() / 2);
        }
    }
}

void CameraController::UpdateCameraAcceleration()
{
    auto acceleration = mCamera->GetAcceleration();
    float movementSpeed = mCamera->GetAccelerationSpeed();

    if (mKeyStates[GLFW_KEY_W]) acceleration.z += movementSpeed;
    if (mKeyStates[GLFW_KEY_S]) acceleration.z -= movementSpeed;
    if (mKeyStates[GLFW_KEY_A]) acceleration.x -= movementSpeed;
    if (mKeyStates[GLFW_KEY_D]) acceleration.x += movementSpeed;
    if (mKeyStates[GLFW_KEY_Q]) acceleration.y -= movementSpeed;
    if (mKeyStates[GLFW_KEY_E]) acceleration.y += movementSpeed;

    mCamera->SetAcceleration(acceleration);
}

void CameraController::HandleKeyboard(Window* window, int key, int scancode, int action, int mods)
{
    LOG_INFO("CameraController::HandleKeyboard");

    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
    {
        mKeyStates[key] = true;
    }
    else if (action == GLFW_RELEASE) 
    {
        mKeyStates[key] = false;
    }
}

void CameraController::HandleMouseScroll(Window* window, double xoffset, double yoffset){}