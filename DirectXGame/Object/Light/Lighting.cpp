#include "Lighting.h"

void Lighting::Initialize() {
	directX12 = DirectX12::GetInstance();

	directionalLightResource = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(DirectionalLight));
	light = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&light));

	light->color = { 1.0f,1.0f,1.0f,1.0f };
	light->direction = { 0.0f,-1.0f,0.0f };
	light->intensity = 1.0f;
}

void Lighting::Update() {

	ImGui::Begin("Light");
	ImGui::DragFloat4("color", &light->color.x);
	ImGui::SliderFloat3("direction", &light->direction.x, -1.0f, 1.0f);
	ImGui::DragFloat("intensity", &light->intensity);
	ImGui::End();

	light->direction = Normalize(light->direction);

	directX12->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
}