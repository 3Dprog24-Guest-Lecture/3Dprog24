#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Renderer.h>
#include <string>

class Scene;

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
    void FramebufferSizeCallback(class GLFWwindow* window, int width, int height);
    void MouseMoveCallback(class GLFWwindow* window, double xpos, double ypos);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void MouseScrollCallback(class GLFWwindow* window, double xoffset, double yoffset);
    void KeyCallback(class GLFWwindow* window, int key, int scancode, int action, int mods);

    // Setters
    void SetWidth(int width);
    void SetHeight(int height);

    // Getters
    int GetWidth() const;
    int GetHeight() const;
    class GLFWwindow* GetGLFWWindow() const { return mGLFWWindow; };

private:
    std::string mName;
    int mWidth, mHeight;
    
    Scene* mScene{nullptr};
    class GLFWwindow* mGLFWWindow{ nullptr };
};