#include <Scene.h>
#include <Logger.h>
#include <Mesh.h>
#include <Material.h>
#include <Shader.h>
#include <Camera.h>
#include <Actor.h>
#include <Defines.h>
#include <CameraController.h>
#include <ActorController.h>
#include <Renderer.h>
#include <imgui.h>
#include <GLFW/glfw3.h>

Scene::Scene(const std::string& name)
	:mSceneGraph(name){}

void Scene::LoadContent()
{
	LOG_INFO("Scene::LoadContent");

	auto tex = Texture::Load(SOURCE_DIRECTORY + "textures/container.jpg");
	auto mat = Material::Load("Default", { tex }, {});

	mCube0 = new MeshActor("Cube0", Mesh::CreateCube(mat));
	mCube1 = new MeshActor("Cube1", Mesh::CreateCube(mat));
	mCube2 = new MeshActor("Cube2", Mesh::CreateCube(mat));
	mShader = new Shader(SOURCE_DIRECTORY + "shaders/shader.vs", SOURCE_DIRECTORY + "shaders/shader.fs");

	mSceneGraph.AddChild(&mSceneCamera);
	mSceneGraph.AddChild(mCube0);
	mSceneGraph.AddChild(mCube1);
	mSceneGraph.AddChild(mCube2);

	mCube0->SetPosition({ -2.f, 0.f, 0.f }, Actor::TransformSpace::Global);
	mCube1->SetPosition({ 2.f, 0.f, 0.f }, Actor::TransformSpace::Global);	
	mSceneCamera.SetPosition({ 0.f, 0.f, 3.f });

	mActorController = std::shared_ptr<ActorController>(new ActorController(mCube0));
	mCameraController = std::shared_ptr<CameraController>(new CameraController(&mSceneCamera));

	mActiveController = mCameraController;
}

void Scene::UnloadContent()
{
	delete mShader;
	delete mCube0;
	delete mCube1;
	delete mCube2;

	Mesh::ClearCache();
	Material::ClearCache();
	Texture::ClearCache();	
}

void Scene::UpdateInputController(float dt)
{
	if (mActiveController)
		mActiveController->Update(dt);
}

void Scene::UpdateSceneGraph(Actor* actor, float dt, Transform globalTransform)
{
	if (!actor) return;

	globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->GetTransformMatrix());

	actor->Update(dt);

	const auto& children = actor->GetChildren();
	for (Actor* child : children) 
	{
		UpdateSceneGraph(child, dt, globalTransform);
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

			auto a = iA->GetAABB();
			auto b = iB->GetAABB();

			glm::vec3 mtv{}; // minimum translation vector to resolve the collision		
			if (a.Intersect(b, &mtv)) // This means that the two bounding boxes intersect
			{
				// Move both actors equally at opposite sides by halfing the mtv vector
				actors[i]->SetPosition(actors[i]->GetGlobalPosition() - mtv * 0.5f);
				actors[j]->SetPosition(actors[j]->GetGlobalPosition() + mtv * 0.5f);
			}
		}
	}
}

void Scene::RenderSceneGraph(Actor* actor, float dt, Transform globalTransform)
{
	if (!actor) return;

	globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->GetTransformMatrix());

	if (auto iRender = dynamic_cast<IRender*>(actor))
	{
		mShader->use();
		mShader->setMat4("model", globalTransform.GetTransformMatrix());
		mShader->setMat4("view", mSceneCamera.GetViewMatrix());
		mShader->setMat4("projection", mSceneCamera.GetProjectionMatrix());
		iRender->Draw(mShader);
	}

	const auto& children = actor->GetChildren();
	for (Actor* child : children)
	{
		RenderSceneGraph(child, dt, globalTransform);
	}
}

void Scene::Render(float dt)
{
	glEnable(GL_DEPTH_TEST);
	RenderSceneGraph(&mSceneGraph, dt);

	// Variables for ImGui combo box
	const char* items[] = { "Camera", "Player" };
	static int item_current = 0; // Current item index

	// Inside your GUI rendering function
	ImGui::Begin("Select controller"); // Create a window

	// Combo box
	ImGui::Combo("Select Item", &item_current, items, IM_ARRAYSIZE(items));

	if (item_current == 0)
	{
		mActiveController = mCameraController;
	}
	else if (item_current == 1)
	{
		mActiveController = mActorController;
	}

	ImGui::End(); // End of the window
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