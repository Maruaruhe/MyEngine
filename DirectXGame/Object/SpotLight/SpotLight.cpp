#include "SpotLight.h"
#include <numbers>

void SpotLight::Initialize() {
	spotLightResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Spot));
	light_ = nullptr;
	spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));

	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->direction = { 0.0f,0.0f,1.0f };
	light_->position = { 2.0f,2.0f,2.0f };
	light_->distance = 50.0f;
	light_->intensity = 1.5f;
	light_->decay = 7.0f;
	light_->cosAngle = 0.45f;
}

void SpotLight::Update() {
#ifdef _DEBUG


	ImGui::Begin("SpotLight");
	ImGui::DragFloat4("color", &light_->color.x, 0.1f);
	ImGui::DragFloat3("position", &light_->position.x, 0.1f);
	ImGui::SliderFloat3("direction", &light_->direction.x, -1.0f, 1.0f);
	light_->direction = Normalize(light_->direction);
	ImGui::DragFloat("intensity", &light_->intensity, 0.1f);
	ImGui::DragFloat("distance", &light_->distance, 0.1f);
	ImGui::DragFloat("decay", &light_->decay, 0.1f);
	ImGui::DragFloat("cosAngle", &light_->cosAngle, 0.01f);
	ImGui::End();

#endif // DEBUG
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, spotLightResource_->GetGPUVirtualAddress());
}