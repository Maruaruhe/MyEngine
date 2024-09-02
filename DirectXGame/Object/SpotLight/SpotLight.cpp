#include "SpotLight.h"
#include <numbers>

void SpotLight::Initialize() {
	spotLightResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(Spot));
	light = nullptr;
	spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&light));

	light->color = { 1.0f,1.0f,1.0f,1.0f };
	light->direction = { 0.0f,0.0f,1.0f };
	light->position = { 2.0f,2.0f,2.0f };
	light->intensity = 1.0f;
	light->decay = 2.0f;
	light->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
}

void SpotLight::Update() {
#ifdef _DEBUG


	ImGui::Begin("SpotLight");
	ImGui::DragFloat4("color", &light->color.x, 0.1f);
	ImGui::SliderFloat3("direction", &light->direction.x, -1.0f, 1.0f);
	light->direction = Normalize(light->direction);
	ImGui::DragFloat("intensity", &light->intensity);
	ImGui::End();

#endif // DEBUG
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, spotLightResource->GetGPUVirtualAddress());
}