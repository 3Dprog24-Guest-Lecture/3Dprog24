#include "CameraController.h"
#include <Logger.h>
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Window.h>

void CameraController::HandleMouseMove(Window* window, double xpos, double ypos)
{
    float xoffset = mLastX - xpos;
    float yoffset = mLastY - ypos;
    LOG_INFO("TEST");
    LOG_INFO(xoffset);
    //LOG_INFO(yoffset);

    mLastX = xpos;
    mLastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    float yawRad = glm::radians(xoffset);
    float pitchRad = glm::radians(yoffset);

    glm::quat currentOrientation = mCamera->GetRotation();
    glm::quat yawRotation = glm::angleAxis(yawRad, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchRotation = glm::angleAxis(pitchRad, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat newOrientation = yawRotation * currentOrientation * pitchRotation;
    newOrientation = glm::normalize(newOrientation);
    mCamera->SetRotation(newOrientation);
}

void CameraController::HandleMouseButton(Window* window, int button, int action, int mods)
{ 
    if (button == GLFW_MOUSE_BUTTON_RIGHT) 
    {
        if (action == GLFW_PRESS) 
        {
            mRightMousePressed = true;
            glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double xpos, ypos;
            glfwGetCursorPos(window->GetGLFWWindow(), &xpos, &ypos);
            mLastX = static_cast<float>(xpos);
            mLastY = static_cast<float>(ypos);
        }
        else if (action == GLFW_RELEASE) 
        {
            mRightMousePressed = false;
            glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void CameraController::UpdateCameraAcceleration()
{
    auto acceleration = mCamera->GetAcceleration();
    float movementSpeed = mCamera->GetAccelerationSpeed();

    acceleration = glm::vec3(0.f);

    if (mKeyStates[GLFW_KEY_W]) acceleration.z += movementSpeed;
    if (mKeyStates[GLFW_KEY_S]) acceleration.z -= movementSpeed;
    if (mKeyStates[GLFW_KEY_A]) acceleration.x -= movementSpeed;
    if (mKeyStates[GLFW_KEY_D]) acceleration.x += movementSpeed;
    if (mKeyStates[GLFW_KEY_Q]) acceleration.y += movementSpeed;
    if (mKeyStates[GLFW_KEY_E]) acceleration.y -= movementSpeed;

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

    UpdateCameraAcceleration();
}

void CameraController::HandleMouseScroll(Window* window, double xoffset, double yoffset){}