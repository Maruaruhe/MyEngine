#include "Camera.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"

void Camera::Initialize() {
	input = Input::GetInstance();

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	horizontalAngle = 0.45f;
	aspectRatio = (float(kClientWidth) / float(kClientHeight));
	nearClip = 0.1f;
	farClip = 100.0f;

	cameraForGPUResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(CameraForGPU));
	position = nullptr;
	cameraForGPUResource->Map(0, nullptr, reinterpret_cast<void**>(&position));
}

void Camera::Update() {
	Vector3 move{};

	if (input->PushKey(DIK_W)) {
		move.z += 0.1f;
	}
	if (input->PushKey(DIK_S)) {
		move.z -= 0.1f;
	}
	if (input->PushKey(DIK_A)) {
		move.x -= 0.1f;
	}
	if (input->PushKey(DIK_D)) {
		move.x += 0.1f;
	}

	if (input->PushKey(DIK_LEFT)) {
		transform.rotate.y -= 0.03f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		transform.rotate.y += 0.03f;
	}

	Matrix4x4 wM = MakeRotateXYZMatrix(transform.rotate);
	move = {
		move.x * wM.m[0][0] + move.z * wM.m[2][0],
		0,
		move.x * wM.m[0][2] + move.z * wM.m[2][2]

	};

	transform.translate = Add(transform.translate,move);

	position->worldPosition = transform.translate;
	//ImGui::Begin("Camera");
	//ImGui::SliderFloat3("translate", &transform.translate.x, -15, 15);
	//ImGui::SliderFloat3("rotate", &transform.rotate.x, -3.0f, 3.0f);
	//ImGui::SliderFloat3("scale", &transform.scale.x, 1.0f, 10.0f);
	//ImGui::End();

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