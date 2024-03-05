#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Renderer.h>
#include <string>

class Scene;

/**
 * @class Window
 * @brief Represents a window in the application, managing its lifecycle, events, and rendering context.
 *
 * The Window class encapsulates a GLFW window and its associated context. It is responsible for initializing
 * the window, handling input events through callbacks, and managing the render loop. The class also integrates
 * with a Scene object to update and render the content within the window.
 */
class Window
{
public:
    Window(std::string name, Scene* scene, int width, int height);
    ~Window();

    void Init();
    void RegisterWindowCallbacks();
    bool LoadContent();
    void SetScene(Scene* scene);
    void StartFrame();
    void Update(float dt);
    void Render(float dt);
    void EndFrame();

    bool IsClosed();

    // Callbacks
    void FramebufferSizeCallback(struct GLFWwindow* window, int width, int height);
    void MouseMoveCallback(struct GLFWwindow* window, double xpos, double ypos);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void MouseScrollCallback(struct GLFWwindow* window, double xoffset, double yoffset);
    void KeyCallback(struct GLFWwindow* window, int key, int scancode, int action, int mods);

    // Setters
    void SetWidth(int width);
    void SetHeight(int height);

    // Getters
    int GetWidth() const;
    int GetHeight() const;
    struct GLFWwindow* GetGLFWWindow() const { return mGLFWWindow; };

private:
    std::string mName;
    int mWidth, mHeight;
    
    Scene* mScene{nullptr};
    struct GLFWwindow* mGLFWWindow{ nullptr };
};