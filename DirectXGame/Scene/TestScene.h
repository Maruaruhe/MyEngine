#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"

#include "../Object/Sprite/Sprite.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"
#include "../Object/Camera/Camera.h"

class TestScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	Input* input = nullptr;
	Sprite black;
	Sprite title;

	Sphere sphere;
	Transform trans;

	std::unique_ptr<Camera> camera;

	int scroll;
	const int scrollSpeed = 8;
	bool isScroll;
};

