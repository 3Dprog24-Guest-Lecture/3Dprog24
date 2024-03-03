#pragma once

#include <Window.h>
#include <Scene.h>
#include <memory>

/**
 * @brief Represents the main application.
 *
 * This class is responsible for initializing the application,
 * including setting up GLFW, loading content, and running the main application loop.
 * It follows the Singleton pattern to ensure only one instance exists.
 */
class Application
{
    public:
    Application(const Application&) = delete;
    Application(Application&&) = delete;

    static Application* Get();

    void InitializeGLFW();

    void Init();

    void LoadContent();

    int Run();

    private:
    Application() = default;

    Scene mScene{"Scene"};
    Window mWindow{"LearnOpenGL", &mScene, 1280, 720};
};