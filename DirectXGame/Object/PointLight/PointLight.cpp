#include "PointLight.h"
#include <numbers>

void PointLight::Initialize() {
	pointLightResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Point));
	light_ = nullptr;
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));

	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->position = { 2.0f,2.0f,2.0f };
	light_->intensity = 1.5f;
}

void PointLight::Update() {
#ifdef _DEBUG


	ImGui::Begin("SpotLight");
	ImGui::DragFloat4("color", &light->color.x, 0.1f);
	ImGui::DragFloat3("position", &light->position.x, 0.1f);
	ImGui::DragFloat("intensity", &light->intensity, 0.1f);
	ImGui::End();

#endif // DEBUG
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLightResource_->GetGPUVirtualAddress());
}