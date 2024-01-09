#include "Player.h"

void Player::Initialize(DirectX12* directX12, const std::string& filename) {
	input_ = Input::GetInstance();
	model_ = std::make_unique<Model>();
	//model_ = new Model();
	model_->Initialize(directX12 , filename);

	transform_.translate = { 0.0f,0.0f,0.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.scale = { 1.0f,1.0f,1.0f };
}

void Player::Update(Vector4& color, DirectionalLight& directionalLight) {
	Move();
	model_->Update(color, transform_, directionalLight);
}

void Player::Move() {
	if (input_->PushKey(DIK_W)) {
		transform_.translate.y += 0.01f;
	}
	if (input_->PushKey(DIK_S)) {
		transform_.translate.y -= 0.01f;
	}
	if (input_->PushKey(DIK_A)) {
		transform_.translate.x -= 0.01f;
	}
	if (input_->PushKey(DIK_D)) {
		transform_.translate.x += 0.01f;
	}
}

void Player::Draw() {
	model_->Draw();
}