#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"

#include "../Object/Sprite/Sprite.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	Input* input = nullptr;

	Sphere sphere;
	Camera camera;
	DirectionalLight light;

	Transform transform;
	Transform cameraTransform;
	Vector4 color;

	CameraForGPU cameraFor;
};

