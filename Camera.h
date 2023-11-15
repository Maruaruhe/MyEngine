#pragma once
#include "Matrix4x4.h"

class Camera
{
public:
	void Initialize();
	Matrix4x4 MakeWVPMatrix(Transform& transform);
private:
	Transform cameraTransform;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};

