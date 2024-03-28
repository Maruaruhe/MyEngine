#include "Camera.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"

void Camera::Initialize() {
	input = Input::GetInstance();

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
	if (input->PushKey(DIK_W)) {
		transform.translate.z += 0.1f;
	}
	if (input->PushKey(DIK_S)) {
		transform.translate.z -= 0.1f;
	}
	if (input->PushKey(DIK_A)) {
		transform.translate.x -= 0.1f;
	}
	if (input->PushKey(DIK_D)) {
		transform.translate.x += 0.1f;
	}
	if (input->PushKey(DIK_Q)) {
		transform.translate.y += 0.1f;
	}
	if (input->PushKey(DIK_E)) {
		transform.translate.y -= 0.1f;
	}

	position->worldPosition = transform.translate;
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &transform.translate.x, -15, 15);
	ImGui::SliderFloat3("rotate", &transform.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("scale", &transform.scale.x, 1.0f, 10.0f);
	ImGui::End();

	MakeWVPMatrix();

	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource->GetGPUVirtualAddress());
}

void Camera::MakeWVPMatrix() {
	cameraMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	viewMatrix = Inverse(cameraMatrix);
	projectionMatrix = MakePerspectiveFovMatrix(horizontalAngle, aspectRatio, nearClip, farClip);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}