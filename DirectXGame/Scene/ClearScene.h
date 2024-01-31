#pragma once
#include "IScene.h"
#include <memory>

#include "../Base/Input/Input.h"

#include "../Object/Sphere/Sphere.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Light/Lighting.h"

class ClearScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	Input* input = nullptr;
	Sphere sphere;
	Transform trans;

	std::unique_ptr<Camera> camera;
};

