#include "Lighting.h"

void Lighting::Initialize() {
	//light = std::make_unique<DirectionalLight>();
	directionalLightResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(DirectionalLight));
	light = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&light));

	light->color = { 1.0f,1.0f,1.0f,1.0f };
	light->direction = { 0.0f,0.0f,1.0f };
	light->direction = { 0.275f,0.944f,0.181f };
	light->intensity = 0.01f;
}

void Lighting::Update() {
#ifdef _DEBUG


	ImGui::Begin("Light");
	ImGui::DragFloat4("color", &light->color.x, 0.1f);
	ImGui::SliderFloat3("direction", &light->direction.x, -1.0f, 1.0f);
	light->direction = Normalize(light->direction);
	ImGui::DragFloat("intensity", &light->intensity, 0.01f);
	ImGui::End();

#endif // DEBUG
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, directionalLightResource->GetGPUVirtualAddress());
}