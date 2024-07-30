#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"
#include "../Base/GlobalVariables/GlobalVariables.h"

#include "../Object/Sprite/Sprite.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Model/Model.h"
#include "../Object/Anime/Anime.h"
#include "../Object/Particle/Particle.h"

#include "../GameObject/Entity/TestObject/TestObject.h"
#include "../GameObject/Entity/TestJsonObject/TestJsonObject.h"
#include "../Object/Skybox/Skybox.h"

class TestScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	KeyInput* input = nullptr;
	Lighting light;

	std::unique_ptr<Camera> camera2;

	TestObject test;
	TestJsonObject testJson;
	Skybox s;
};

