#pragma once
#include "../../Math/Matrix4x4.h"
#include "../../Base/ImGui/ImGuiWND.h"
#include "../../Base/Input/Input.h"

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

