#include "Camera.h"
void Camera::Initialize() {
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	horizontalAngle = 0.45f;
	aspectRatio = (float(kClientWidth) / float(kClientHeight));
	nearClip = 0.1f;
	farClip = 100.0f;

	cameraForGPUResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(CameraForGPU));
	position = nullptr;
	cameraForGPUResource->Map(0, nullptr, reinterpret_cast<void**>(&position));
}

void Camera::Update() {
	position->worldPosition = transform.translate;
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &transform.translate.x, -15, 15);
	ImGui::SliderFloat3("rotate", &transform.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("scale", &transform.scale.x, 1.0f, 10.0f);
	ImGui::End();

	MakeWVPMatrix();

	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource->GetGPUVirtualAddress());
}

void Camera::MakeWVPMatrix() {
	Matrix4x4 cameraMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(horizontalAngle, aspectRatio, nearClip, farClip);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}