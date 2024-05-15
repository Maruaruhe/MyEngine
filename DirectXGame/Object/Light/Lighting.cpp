#include "Lighting.h"

void Lighting::Initialize() {
	directionalLightResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(DirectionalLight));
	light = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&light));

	light->color = { 1.0f,1.0f,1.0f,1.0f };
	light->direction = { 0.0f,0.0f,1.0f };
	light->intensity = 1.0f;
}

void Lighting::Update() {
	//ImGui::Begin("Light");
	//ImGui::DragFloat4("color", &light->color.x);
	//ImGui::SliderFloat3("direction", &light->direction.x, -1.0f, 1.0f);
	//light->direction = Normalize(light->direction);
	//ImGui::DragFloat("intensity", &light->intensity);
	//ImGui::End();

	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
}