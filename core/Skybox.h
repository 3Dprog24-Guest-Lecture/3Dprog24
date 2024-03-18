#pragma once
#include <Types.h>
#include <array>
#include <string>

class Skybox
{
public:
	Skybox(const std::array<std::string, 6>& texturePaths);
	~Skybox();

	void Render(class CameraActor* renderCamera);
private:
	TextureID mTextureId{0};

	class Mesh* mMesh{ nullptr };
	class Shader* mSkyboxShader{ nullptr };
};