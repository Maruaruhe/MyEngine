#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"
#include "../Base/GlobalVariables/GlobalVariables.h"

#include "../Object/Sprite/Sprite.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Model/Model.h"

class TestScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	Input* input = nullptr;
	Lighting light;

	Camera* camera2;

	Sphere sphere;
	Sprite a;
	//Model model;
	Model model;
	Model modela;
	Sprite sprite;

	//std::unique_ptr<Camera> camera;
};

