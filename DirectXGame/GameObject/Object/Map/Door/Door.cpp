#include "Door.h"
#include "../../../DirectXGame/Manager/TextureManager.h"

using namespace MyEngine;

void Door::Initialize(const Transform transform) {
	ModelManager::GetInstance()->LoadModel("2x2cube");

	model_.Initialize("2x2cube");
	model_.transform = transform;

	model_.material->enableLighting = true;
}

void Door::Update() {
	model_.Update();
}

void Door::Draw() {
	model_.Draw();
}