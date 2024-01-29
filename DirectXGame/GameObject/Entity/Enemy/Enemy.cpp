#include "Enemy.h"

void Enemy::Initialize(const std::string& filename) {
	input_ = Input::GetInstance();
	model_ = std::make_unique<Model>();

	model_->Initialize(filename);

	transform_.translate = { 0.0f,0.0f,25.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.scale = { 0.5f,0.5f,0.5f };
}

void Enemy::Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight) {
	model_->Update(color, transform_, cameraTransform, directionalLight);
}

void Enemy::Draw() {
	if (isAlive) {
		model_->Draw();
	}
}