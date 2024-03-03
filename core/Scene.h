#pragma once
#include <Camera.h>
#include <SceneGraph.h>
#include <memory>
#include <Controller.h>
#include <Mesh.h>

class Scene
{
public:
    explicit Scene(const std::string& name);
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    virtual void LoadContent();
    virtual void UnloadContent();
    void UpdateSceneGraph(Actor* actor, float dt, Transform globalTransform = Transform{});
    void RenderSceneGraph(Actor* actor, float dt, Transform globalTransform = Transform{});
    void Update(float dt);
    void Render(float dt);

    void FramebufferSizeCallback(class Window* window, int width, int height);
    void MouseMoveCallback(class Window* window, double xpos, double ypos);
    void MouseButtonCallback(class Window* window, int button, int action, int mods);
    void MouseScrollCallback(class Window* window, double xoffset, double yoffset);
    void KeyCallback(class Window* window, int key, int scancode, int action, int mods);

    std::shared_ptr<IController> GetController() const { return mController; }
    void SetController(const std::shared_ptr<IController>& controller) { mController = controller; }

    SceneGraph mSceneGraph;
    Camera mSceneCamera{ "SceneCamera" };

private:
    MeshActor* mCube0{nullptr};
    MeshActor* mCube1{nullptr};
    class Shader* mShader{ nullptr };

protected:
    std::shared_ptr<IController> mController{ nullptr };
};