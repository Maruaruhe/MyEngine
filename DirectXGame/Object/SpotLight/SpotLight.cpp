#include "SpotLight.h"
#include <numbers>

void SpotLight::Initialize() {
	spotLightResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(Spot));
	light = nullptr;
	spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&light));

	light->color = { 1.0f,1.0f,1.0f,1.0f };
	light->direction = { 0.0f,0.0f,1.0f };
	light->position = { 2.0f,2.0f,2.0f };
	light->distance = 100.0f;
	light->intensity = 10.0f;
	light->decay = 10.0f;
	light->cosAngle = 0.45f;
}

void SpotLight::Update() {
#ifdef _DEBUG


	ImGui::Begin("SpotLight");
	ImGui::DragFloat4("color", &light->color.x, 0.1f);
	ImGui::DragFloat3("position", &light->position.x, 0.1f);
	ImGui::SliderFloat3("direction", &light->direction.x, -1.0f, 1.0f);
	light->direction = Normalize(light->direction);
	ImGui::DragFloat("intensity", &light->intensity, 0.1f);
	ImGui::DragFloat("distance", &light->distance, 0.1f);
	ImGui::DragFloat("decay", &light->decay, 0.1f);
	ImGui::DragFloat("cosAngle", &light->cosAngle, 0.01f);
	ImGui::End();

#endif // DEBUG
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, spotLightResource->GetGPUVirtualAddress());
}