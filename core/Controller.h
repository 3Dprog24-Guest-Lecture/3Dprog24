#pragma once

class IController
{
public:
    virtual ~IController() = default;

    // Virtual methods for handling input events
    virtual void HandleMouseMove(class Window* window, double xpos, double ypos) = 0;
    virtual void HandleMouseScroll(class Window* window, double xoffset, double yoffset) = 0;
    virtual void HandleMouseButton(class Window* window, int button, int action, int mods) = 0;
    virtual void HandleKeyboard(class Window* window, int key, int scancode, int action, int mods) = 0;
};