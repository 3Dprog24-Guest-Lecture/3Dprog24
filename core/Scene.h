#pragma once
#include <Camera/Camera.h>
#include <SceneGraph.h>
#include <memory>
#include <Controller.h>
#include <Mesh.h>
#include <ActorController.h>
#include <Camera/CameraController.h>
#include <Lights/PointLight.h>
#include <Lights/DirectionalLight.h>

/**
 * @brief Represents a scene containing actors, lights, and a camera.
 *
 * Manages the lifecycle, rendering, and interactions of all components within the scene. It orchestrates
 * updating the scene graph, handling user input, managing lights, and rendering the scene and UI.
 */
class Scene
{
public:
    /**
     * @brief Constructs a scene with a specified name.
     *
     * @param name The name of the scene.
     */
    Scene(const std::string& name);

    virtual ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    /**
     * @brief Loads content associated for the scene.
     */
    virtual void LoadContent();

    /**
     * @brief Unloads content associated with the scene.
     */
    virtual void UnloadContent();

    /**
     * @brief Updates the active controller based on input.
     *
     * @param dt Delta time for the update.
     */
    void UpdateInputController(float dt);

    /**
     * @brief Recursively updates actors within the scene graph.
     *
     * @param[in] actor The current actor to update.
     * @param dt Delta time for the update.
     * @param globalParentTransform The global parent transform for the actor.
     */
    void UpdateSceneGraph(Actor* actor, float dt, Transform globalParentTransform = Transform{});

    /**
     * @brief Updates the scene, including input, scene graph and handling collisions.
     *
     * @param dt Delta time for the update.
     */
    void Update(float dt);

    /**
     * @brief Handles collision detection and response within the scene.
     */
    void HandleCollision();

    /**
     * @brief Renders actors within the scene graph.
     *
     * @param actor The current actor to render.
     * @param dt Delta time for the rendering.
     * @param globalTransform The global transform for the actor.
     */
    void RenderSceneGraph(Actor* actor, float dt, Transform globalParentTransform = Transform{});

    /**
     * @brief Binds and sets up the directional light for rendering.
     */
    void BindDirectionalLight();

    /**
     * @brief Binds and sets up point lights for rendering.
     */
    void BindPointLights();

    /**
     * @brief Binds the camera to the renderer.
     */
    void BindCamera();

    /**
     * @brief Renders the UI elements of the scene.
     */
    void RenderUI();

    /**
     * @brief Renders the scene.
     *
     * @param dt Delta time for the rendering.
     */
    void Render(float dt);

    // Callbacks for window events
    void FramebufferSizeCallback(class Window* window, int width, int height);
    void MouseMoveCallback(class Window* window, double xpos, double ypos);
    void MouseButtonCallback(class Window* window, int button, int action, int mods);
    void MouseScrollCallback(class Window* window, double xoffset, double yoffset);
    void KeyCallback(class Window* window, int key, int scancode, int action, int mods);

    /**
     * @brief Sets the active controller for the scene.
     *
     * @param controller The controller to activate.
     */
    void SetController(const std::shared_ptr<IController>& controller) { mActiveController = controller; };

    /**
     * @brief Retrieves the active controller of the scene.
     *
     * @return The active controller.
     */
    std::shared_ptr<IController> GetController() const { return mActiveController; };

    /**
     * @brief The scene graph managing hierarchical relationships of actors.
     */
    SceneGraph mSceneGraph;

    /**
     * @brief The camera used to view the scene.
     */
    CameraActor mSceneCamera{ "SceneCamera" };

private:
    MeshActor* mCubeActor0{ nullptr };
    StaticMeshActor* mStaticMeshActor0{ nullptr };

    class Skybox* mSkybox{ nullptr };
    class Shader* mShader{ nullptr };

    PointLightActor* mPointLightActor{ nullptr };
    DirectionalLightActor* mDirectionalLightActor{ nullptr };

    std::shared_ptr<ActorController> mActorController;
    std::shared_ptr<CameraController> mCameraController;

protected:
    /**
     * @brief The active controller to update.
     */
    std::shared_ptr<IController> mActiveController{ nullptr };
};