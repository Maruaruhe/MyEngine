#include "Bullet.h"

void Bullet::Initialize(const std::string& filename, Vector3& position, Vector3& velocity) {
	model_ = std::make_unique<Model>();

	model_->Initialize(filename);

	/*ball_ = std::make_unique<Sphere>();
	ball_->Initialize(directX12);*/

	transform_.translate = {0,2,4};
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.scale = { 0.5f,0.5f,2.0f };

	velocity_ = velocity;
}

void Bullet::Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight) {
	Move();
	model_->Update(color, transform_, cameraTransform, directionalLight);
	//ball_->Update(color, transform_, cameraTransform, directionalLight);
}

void Bullet::Move() {
	transform_.translate.x += velocity_.x;
	transform_.translate.y += velocity_.y;
	transform_.translate.z += velocity_.z;
}

void Bullet::Draw() {
	model_->Draw();
	//ball_->Draw();
}