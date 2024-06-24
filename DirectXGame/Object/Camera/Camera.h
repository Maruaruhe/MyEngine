#pragma once
#include "../../Math/Matrix4x4.h"
#include "../../Base/ImGui/ImGuiWND.h"

#include "../../Base/Input/Input.h"

#include <d3d12.h>
#include "../../Base/DirextX12/DirectX12.h"
#include "../../Math/Vector4.h"
#include "../../Math/struct.h"	
#include "../../Math/Matrix4x4.h"
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

struct CameraForGPU {
	Vector3 worldPosition;
};

class Camera
{
public:
	void Initialize();
	void Update();
	void MakeWVPMatrix();

	CameraForGPU* position;
	Transform transform;

	Matrix4x4 viewProjectionMatrix;

	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
private:
	KeyInput* input = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGPUResource;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;


	float horizontalAngle;
	float aspectRatio;
	float nearClip;
	float farClip;
};

