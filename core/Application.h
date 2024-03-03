#pragma once

#include <Window.h>
#include <Scene.h>
#include <memory>

class Application
{
    public:
    Application(const Application&) = delete;
    Application(Application&&) = delete;

    static Application* Get();

    void InitializeGLFW();

    void Init();

    void LoadContent();

    void Run();

    private:
    Application() = default;

    Scene mScene{"Scene"};
    Window mWindow{"LearnOpenGL", &mScene, 1280, 720};
};