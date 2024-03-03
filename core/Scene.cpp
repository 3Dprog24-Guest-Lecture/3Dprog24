#include <Scene.h>
#include <Logger.h>
#include <Mesh.h>
#include <Material.h>
#include <Shader.h>
#include <Camera.h>
#include <Actor.h>
#include <Defines.h>
#include <CameraController.h>
#include <Renderer.h>
#include <imgui.h>

Scene::Scene(const std::string& name)
	:mSceneGraph(name){}

void Scene::LoadContent()
{
	LOG_INFO("Scene::LoadContent");

	auto tex = Texture::Load(SOURCE_DIRECTORY + "textures/container.jpg");
	auto mat = Material::Load("Default", { tex }, {});

	mCube0 = new MeshActor("Cube0", Mesh::CreateCube(mat));
	mCube1 = new MeshActor("Cube1", Mesh::CreateCube(mat));

	mSceneGraph.AddChild(&mSceneCamera);
	mSceneGraph.AddChild(mCube0);
	mCube0->AddChild(mCube1);

	mCube0->SetPosition({ -1.f, 0.f, 0.f }, Actor::TransformSpace::Global);
	mCube1->SetPosition({ 1.f, 0.f, 0.f }, Actor::TransformSpace::Global);

	mSceneCamera.SetPosition({ 0.f, 0.f, 3.f });
	mController = std::shared_ptr<CameraController>(new CameraController(&mSceneCamera));

	mShader = new Shader(SOURCE_DIRECTORY + "shaders/shader.vs", SOURCE_DIRECTORY + "shaders/shader.fs");
}

void Scene::UnloadContent()
{
	delete mShader;
	delete mCube0;
	delete mCube1;

	Mesh::ClearCache();
	Material::ClearCache();
	Texture::ClearCache();	
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

void Scene::Update(float dt)
{
	UpdateSceneGraph(&mSceneGraph, dt);
}

void Scene::Render(float dt)
{
	static bool bShowDemoWindow = true;
	if (bShowDemoWindow)
	{ 
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}

	glEnable(GL_DEPTH_TEST);
	RenderSceneGraph(&mSceneGraph, dt);
}

void Scene::MouseMoveCallback(Window* window, double xpos, double ypos)
{
	if (mController)
		mController->HandleMouseMove(window, xpos, ypos);
}

void Scene::MouseButtonCallback(Window* window, int button, int action, int mods)
{
	if (mController)
		mController->HandleMouseButton(window, button, action, mods);
}

void Scene::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{
	if (mController)
		mController->HandleMouseScroll(window, xoffset, yoffset);
}

void Scene::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{
	if (mController)
		mController->HandleKeyboard(window, key, scancode, action, mods);
}

void Scene::FramebufferSizeCallback(Window* window, int width, int height) 
{
	mSceneCamera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}