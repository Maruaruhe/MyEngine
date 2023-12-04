#pragma once
#include "Matrix4x4.h"
#include "ImGuiWND.h"

class Camera
{
public:
	void Initialize();
	void Update();
	Matrix4x4 MakeWVPMatrix(Transform& transform);

	Transform GetTransform() { return cameraTransform; }
private:
	Transform cameraTransform;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};

