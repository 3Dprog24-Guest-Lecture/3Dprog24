#include <Scene.h>
#include <Logger.h>
#include <Mesh.h>
#include <Material.h>
#include <Shader.h>
#include <Camera/Camera.h>
#include <Actor.h>
#include <Defines.h>
#include <Camera/CameraController.h>
#include <ActorController.h>
#include <Renderer.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <Physics/PhysicsComponent.h>
#include <Skybox/Skybox.h>
#include <ModelLoader/AssimpLoader.h>

Scene::Scene(const std::string& name)
	:mSceneGraph(name){}

void Scene::LoadContent()
{
	LOG_INFO("Scene::LoadContent");

	Texture* diffuseTex = Texture::Load(SOURCE_DIRECTORY("assets/textures/container2.jpg"));
	Texture* specularTex = Texture::Load(SOURCE_DIRECTORY("assets/textures/container2_specular.jpg"));
	Material* mat = Material::Load("Default", { diffuseTex, specularTex }, {});

	mCubeActor0 = new MeshActor("Cube0", Mesh::LoadCube(mat));

	mPointLightActor = new PointLightActor("PointLight0");
	mDirectionalLightActor = new DirectionalLightActor("DirectionalLight0");

	mStaticMeshActor0 = new StaticMeshActor("StaticMeshActor0");
	AssimpLoader::Load(SOURCE_DIRECTORY("assets/models/sponza/sponza.fbx"), mStaticMeshActor0);

	mSkybox = new Skybox({
		SOURCE_DIRECTORY("assets/textures/starfield_And_Haze/Starfield_And_Haze_left.png"),
		SOURCE_DIRECTORY("assets/textures/starfield_And_Haze/Starfield_And_Haze_right.png"),
		SOURCE_DIRECTORY("assets/textures/starfield_And_Haze/Starfield_And_Haze_up.png"),
		SOURCE_DIRECTORY("assets/textures/starfield_And_Haze/Starfield_And_Haze_down.png"),
		SOURCE_DIRECTORY("assets/textures/starfield_And_Haze/Starfield_And_Haze_front.png"),
		SOURCE_DIRECTORY("assets/textures/starfield_And_Haze/Starfield_And_Haze_back.png"),
		});

	mShader = new Shader(SOURCE_DIRECTORY("assets/shaders/shader.vs"), SOURCE_DIRECTORY("assets/shaders/shader.fs"));

	mSceneGraph.AddChild(&mSceneCamera);
	mSceneGraph.AddChild(mCubeActor0);
	mSceneGraph.AddChild(mStaticMeshActor0);
	//mSceneGraph.AddChild(mPointLightActor);
	//mSceneGraph.AddChild(mDirectionalLightActor);

	mCubeActor0->AddComponent<PhysicsComponent>("Cube0PhysicsComponent");
	mCubeActor0->mCollisionProperties.mType = CollisionType::DYNAMIC;
	mCubeActor0->SetWorldPosition({ 0.f, 10.f, 0.f });

	mSceneCamera.SetWorldPosition({ 0.f, 2.f, 3.f });
	mSceneCamera.SetPitch(-30.f);

	mDirectionalLightActor->SetWorldRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	//mPointLightActor->SetPosition({ 0.f, 2.0f, 0.f });

	mActorController = std::shared_ptr<ActorController>(new ActorController(mCubeActor0));
	mCameraController = std::shared_ptr<CameraController>(new CameraController(&mSceneCamera));

	mActiveController = mCameraController;
}

void Scene::UnloadContent()
{
	delete mShader;
	mShader = nullptr;
	delete mCubeActor0;
	mCubeActor0 = nullptr;
	delete mStaticMeshActor0;
	mStaticMeshActor0 = nullptr;
	delete mPointLightActor;
	mPointLightActor = nullptr;
	delete mDirectionalLightActor;
	mDirectionalLightActor = nullptr;
	delete mSkybox;
	mSkybox = nullptr;
	
	Mesh::ClearCache();
	Material::ClearCache();
	Texture::ClearCache();	
}

void Scene::UpdateInputController(float dt)
{
	if (mActiveController)
		mActiveController->Update(dt);
}

void Scene::UpdateSceneGraph(Actor* actor, float dt, Transform globalParentTransform)
{
	if (!actor) return;

	actor->Update(dt);
	actor->UpdateComponents(dt);

	globalParentTransform.SetTransformMatrix(globalParentTransform.GetTransformMatrix() * actor->GetLocalTransformMatrix());

	const auto& children = actor->GetChildren();
	for (Actor* child : children) 
	{
		UpdateSceneGraph(child, dt, globalParentTransform);
	}
}

void Scene::Update(float dt)
{
	// Update input first
	UpdateInputController(dt);

	// Update the scene
	UpdateSceneGraph(&mSceneGraph, dt);

	// Then handle collision
	HandleCollision();
}

void Scene::HandleCollision()
{
	std::vector<Actor*> actors;
	mSceneGraph.Query<IBounded>(actors);

	// Check and resolve collisions
	for (auto i = 0; i < actors.size(); i++)
	{
		for (auto j = i + 1; j < actors.size(); j++)
		{
			IBounded* iA = dynamic_cast<IBounded*>(actors[i]);
			IBounded* iB = dynamic_cast<IBounded*>(actors[j]);

			// Skip intersection if a object ignores collision
			if (iA->GetCollisionProperties().IsIgnoreResponse() ||
				iB->GetCollisionProperties().IsIgnoreResponse())
			{
				continue;
			}

			// Skip intersection checks for two static objects
			if (iA->GetCollisionProperties().IsStatic() &&
				iB->GetCollisionProperties().IsStatic())
			{ 
				continue;
			}

			auto a = iA->GetAABB();
			auto b = iB->GetAABB();

			glm::vec3 mtv{}; // minimum translation vector to resolve the collision		
			if (a.Intersect(b, &mtv)) // This means that the two bounding boxes intersect
			{
				// Determine how to apply the MTV based on the collision responses of the actors
				bool isADynamic = iA->GetCollisionProperties().IsDynamic();
				bool isBDynamic = iB->GetCollisionProperties().IsDynamic();

				glm::vec3 mtvA(0.f), mtvB(0.f); // Initialize MTV adjustments

				if (isADynamic && isBDynamic)
				{
					// If both actors are dynamic, split the MTV between them
					mtvA = -mtv * 0.5f;
					mtvB = mtv * 0.5f;
				}
				else if (isADynamic)
				{
					// If only actor A is dynamic, apply the full MTV to A
					mtvA = -mtv;
				}
				else if (isBDynamic)
				{
					// If only actor B is dynamic, apply the full MTV to B
					mtvB = mtv;
				}
				// No adjustment for static objects

				// Apply MTV adjustments
				if (isADynamic)
				{
					actors[i]->SetWorldPosition(actors[i]->GetWorldPosition() + mtvA);
				}
				if (isBDynamic)
				{
					actors[j]->SetWorldPosition(actors[j]->GetWorldPosition() + mtvB);
				}
			}
		}
	}
}

void Scene::RenderSceneGraph(Actor* actor, float dt, Transform globalParentTransform)
{
	if (!actor) return;

	globalParentTransform.SetTransformMatrix(globalParentTransform.GetTransformMatrix() * actor->GetLocalTransformMatrix());

	if (auto iRender = dynamic_cast<IRender*>(actor))
	{
		mShader->setMat4("model", globalParentTransform.GetTransformMatrix());
		iRender->Draw(mShader);
	}

	const auto& children = actor->GetChildren();
	for (Actor* child : children)
	{
		RenderSceneGraph(child, dt, globalParentTransform);
	}
}

void Scene::BindDirectionalLight()
{
	// Bind Directional light
	std::vector<Actor*> directionalLights;
	mSceneGraph.Query<DirectionalLightActor>(directionalLights);
	if (!directionalLights.empty())
	{
		auto dl = dynamic_cast<DirectionalLightActor*>(directionalLights[0]);
		mShader->setVec3("dl.direction", glm::normalize(dl->GetDirection()));
		mShader->setVec3("dl.color", dl->mColor);
		mShader->setVec3("dl.ambient", dl->mAmbient);
	}
}

void Scene::BindPointLights()
{
	// Bind Point lights
	std::vector<Actor*> pointLightActors;
	mSceneGraph.Query<PointLightActor>(pointLightActors);

	mShader->setInt("numPointLights", static_cast<int>(pointLightActors.size()));
	for (int i = 0; i < pointLightActors.size(); i++)
	{
		auto pl = dynamic_cast<PointLightActor*>(pointLightActors[i]);

		std::string pointLightArrayIndex = "pointLights[" + std::to_string(i) + "]";
		mShader->setVec3(pointLightArrayIndex + ".ambient", pl->mAmbient);
		mShader->setVec3(pointLightArrayIndex + ".color", pl->mColor);
		mShader->setVec3(pointLightArrayIndex + ".position", pl->GetWorldPosition());
		mShader->setFloat(pointLightArrayIndex + ".constant", pl->constant);
		mShader->setFloat(pointLightArrayIndex + ".linear", pl->linear);
		mShader->setFloat(pointLightArrayIndex + ".quadratic", pl->quadratic);
	}
}

void Scene::BindCamera()
{
	mShader->setMat4("view", mSceneCamera.GetViewMatrix());
	mShader->setMat4("projection", mSceneCamera.GetProjectionMatrix());
	mShader->setVec3("viewPos", mSceneCamera.GetWorldPosition());
}

void Scene::RenderUI()
{
	ImGui::Begin("Settings"); // Create a window
	{ // Select controller
		// Variables for ImGui combo box
		const char* items[] = { "Camera", "Player" };
		static int item_current = 0; // Current item index

		// Combo box
		ImGui::Combo("Select controller", &item_current, items, IM_ARRAYSIZE(items));

		if (item_current == 0)
		{
			mActiveController = mCameraController;
		}
		else if (item_current == 1)
		{
			mActiveController = mActorController;
		}
	}
	{ // Select scene
	// Variables for ImGui combo box
		const char* items[] = { "Sponza", "CollisionTest", "Scene hierarchy test", "MegaScans test"};
		static int item_current = 0; // Current item index

		// Combo box
		if (ImGui::Combo("Select scene", &item_current, items, IM_ARRAYSIZE(items)))
		{ 
			if (mStaticMeshActor0)
			{
				mSceneGraph.RemoveChild(mStaticMeshActor0);
				delete mStaticMeshActor0;
				mStaticMeshActor0 = new StaticMeshActor("StaticMeshActor0");
			};

			if (item_current == 0)
			{
				AssimpLoader::Load(SOURCE_DIRECTORY("assets/models/sponza/sponza.fbx"), mStaticMeshActor0);
			}
			else if (item_current == 1)
			{
				AssimpLoader::Load(SOURCE_DIRECTORY("assets/models/collisionTest/collisionTest.fbx"), mStaticMeshActor0);
			}
			else if (item_current == 2)
			{
				AssimpLoader::Load(SOURCE_DIRECTORY("assets/models/sceneHierarchy/sceneHierarchy.fbx"), mStaticMeshActor0);
			}
			else if (item_current == 3)
			{			
				AssimpLoader::Load(SOURCE_DIRECTORY("assets/models/megascansStone/megascansStone.fbx"), mStaticMeshActor0);
			}

			mCubeActor0->SetWorldPosition({ 0.f, 10.f, 0.f });
			mSceneGraph.AddChild(mStaticMeshActor0);
		}
	}
	ImGui::End(); // End of the window
}

void Scene::Render(float dt)
{
	glEnable(GL_DEPTH_TEST);
	// Bind Shader, only using 1 shader for now
	mShader->use();
	BindDirectionalLight();
	BindPointLights();
	BindCamera();
	RenderSceneGraph(&mSceneGraph, dt);
	RenderUI();

	glDepthFunc(GL_LEQUAL);
	mSkybox->Render(&mSceneCamera);
}

void Scene::MouseMoveCallback(Window* window, double xpos, double ypos)
{
	if (mActiveController)
		mActiveController->HandleMouseMove(window, xpos, ypos);
}

void Scene::MouseButtonCallback(Window* window, int button, int action, int mods)
{
	if (mActiveController)
		mActiveController->HandleMouseButton(window, button, action, mods);
}

void Scene::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{
	if (mActiveController)
		mActiveController->HandleMouseScroll(window, xoffset, yoffset);
}

void Scene::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{
	if (mActiveController)
		mActiveController->HandleKeyboard(window, key, scancode, action, mods);
}

void Scene::FramebufferSizeCallback(Window* window, int width, int height) 
{
	mSceneCamera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}