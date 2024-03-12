#pragma once
#include <Types.h>
#include <initializer_list>
#include <string>

class Skybox
{
public:
	Skybox(std::initializer_list<std::string> texturePaths);
	~Skybox();

	void Render(class CameraActor* renderCamera);
private:
	TextureID mTextureId{0};

	class Mesh* mMesh{ nullptr };
	class Shader* mSkyboxShader{ nullptr };
};