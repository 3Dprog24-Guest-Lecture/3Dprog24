#pragma once

#include <Controller.h>
#include <map>

class CameraController : public IController
{
public:
    CameraController(class Camera* camera) : mCamera(camera) {}

    virtual void HandleMouseMove(class Window* window, double xpos, double ypos) override;
    virtual void HandleMouseScroll(class Window* window, double xoffset, double yoffset) override;
    virtual void HandleMouseButton(class Window* window, int button, int action, int mods) override;
    virtual void HandleKeyboard(class Window* window, int key, int scancode, int action, int mods) override;

private:
    void UpdateCameraAcceleration();

    float mLastX = 0.0, mLastY = 0.0;
    bool mRightMousePressed = false;
    float mouseSensitivity = 0.1f;

    std::map<int, bool> mKeyStates;
    class Camera* mCamera;
};