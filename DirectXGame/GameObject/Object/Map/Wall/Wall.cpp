#include "Wall.h"

void Wall::Initialize(const Vector3 pos, const Vector3 scale) {
	ModelManager::GetInstance()->LoadModel("box");
	model.Initialize("box");
	model.transform.translate = pos;
	model.transform.scale = scale;
}

void Wall::Update() {
	model.Update();

	ImGui::Begin("uvTexture");
	ImGui::DragFloat4("a", model.material->uvTransform.m[0]);
	ImGui::DragFloat4("b", model.material->uvTransform.m[1]);
	ImGui::DragFloat4("c", model.material->uvTransform.m[2]);
	ImGui::DragFloat4("d", model.material->uvTransform.m[3]);
	ImGui::End();
}

void Wall::Draw() {
	model.Draw();
}