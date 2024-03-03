#pragma once

/**
 * @brief Interface for input controllers.
 *
 * The IController interface defines the structure for input handling in the application.
 * It includes methods for processing mouse movement, mouse scrolling, mouse button actions,
 * and keyboard actions. A controllable object can inherit from this, only one active controller will proccess the inputs.
 * This can be used for camera, player or whatever object that needs to be controlled.
 */
class IController
{
public:
    virtual ~IController() = default;

    virtual void HandleMouseMove(class Window* window, double xpos, double ypos) = 0;
    virtual void HandleMouseScroll(class Window* window, double xoffset, double yoffset) = 0;
    virtual void HandleMouseButton(class Window* window, int button, int action, int mods) = 0;
    virtual void HandleKeyboard(class Window* window, int key, int scancode, int action, int mods) = 0;
};