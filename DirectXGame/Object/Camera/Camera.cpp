#include "Camera.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"

using namespace MyEngine;

void Camera::Initialize() {
	input = KeyInput::GetInstance();

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,2.0f,0.0f} };
	//����p����
	fov = 0.85f;
	aspectRatio = (float(kClientWidth) / float(kClientHeight));
	nearClip = 0.1f;
	farClip = 100.0f;

	cameraForGPUResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(CameraForGPU));
	position = nullptr;
	cameraForGPUResource->Map(0, nullptr, reinterpret_cast<void**>(&position));
}

void Camera::Update() {

	position->worldPosition = transform.translate;

#ifdef _DEBUG


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &transform.translate.x, -15, 15);
	ImGui::SliderFloat3("rotate", &transform.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("scale", &transform.scale.x, 1.0f, 10.0f);
	ImGui::End();

#endif // DEBUG
	MakeWVPMatrix();

//	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraForGPUResource->GetGPUVirtualAddress());
}

void Camera::MakeWVPMatrix() {
	cameraMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(fov, aspectRatio, nearClip, farClip);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}