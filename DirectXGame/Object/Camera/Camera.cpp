#include "Camera.h"
void Camera::Initialize() {
	cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	cameraForGPUResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(CameraForGPU));
	position = nullptr;
	cameraForGPUResource->Map(0, nullptr, reinterpret_cast<void**>(&position));
}

void Camera::Update() {
	position->worldPosition = cameraTransform.translate;
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &cameraTransform.translate.x, -15, 15);
	ImGui::SliderFloat3("rotate", &cameraTransform.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("scale", &cameraTransform.scale.x, 1.0f, 10.0f);
	ImGui::End();

	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource->GetGPUVirtualAddress());
}

Matrix4x4 Camera::MakeWVPMatrix(Transform& transform) {
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	return worldViewProjectionMatrix;
}