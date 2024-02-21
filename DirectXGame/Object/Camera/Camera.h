#pragma once
#include "../../Math/Matrix4x4.h"
#include "../../Base/ImGui/ImGuiWND.h"
#include "../../Base/Input/Input.h"

struct CameraForGPU {
	Vector3 worldPosition;
};

class Camera
{
public:
	void Initialize();
	void Update();
	Matrix4x4 MakeWVPMatrix(Transform& transform);

	CameraForGPU position;
private:
	Transform cameraTransform;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};

