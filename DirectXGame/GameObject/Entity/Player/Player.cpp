#include "Player.h"

void Player::Initialize(const std::string& filename) {
	input_ = Input::GetInstance();

	model_ = std::make_unique<Model>();
	reticleModel_ = std::make_unique<Model>();
	bulletModel_ = std::make_unique<Model>();
	bullet = std::make_unique<Sphere>();

	model_->Initialize(filename);
	reticleModel_->Initialize(filename);
	bulletModel_->Initialize(filename);
	bullet->Initialize();

	//bullet_ = std::make_unique<Bullet>();

	transform_.translate = { 0.0f,0.0f,0.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.scale = { 1.0f,1.0f,1.0f };
	
	reticleTransform_.translate = { 0.0f,0.0f,25.0f };
	reticleTransform_.rotate = { 0.0f,0.0f,0.0f };
	reticleTransform_.scale = { 0.5f,0.5f,0.5f };

	bulletTransform_.translate = { -10.0f,0.0f,0.0f };
	bulletTransform_.rotate = { 0.0f,0.0f,0.0f };
	bulletTransform_.scale = { 0.3f,0.3f,0.3f };
	
	bu.translate = { -10.0f,0.0f,0.0f };
	bu.rotate = { 0.0f,0.0f,0.0f };
	bu.scale = { 0.3f,0.3f,0.3f };
}

void Player::Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight) {
	Move();
	ReticleMove();
	Attack();
	BulletMove();

	model_->Update(color, transform_,cameraTransform, directionalLight);
	reticleModel_->Update(color, reticleTransform_,cameraTransform, directionalLight);
	bulletModel_->Update(color, bulletTransform_,cameraTransform, directionalLight);
	bullet->Update(color,bu,cameraTransform,directionalLight);

	//bullet_->Update(color,cameraTransform,directionalLight);
}

void Player::Move() {
	if (input_->PushKey(DIK_W)) {
		transform_.translate.y += 0.03f;
	}
	if (input_->PushKey(DIK_S)) {
		transform_.translate.y -= 0.03f;
	}
	if (input_->PushKey(DIK_A)) {
		transform_.translate.x -= 0.03f;
	}
	if (input_->PushKey(DIK_D)) {
		transform_.translate.x += 0.03f;
	}
}

void Player::ReticleMove() {
	if (input_->PushKey(DIK_UP)) {
		reticleTransform_.translate.y += 0.03f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		reticleTransform_.translate.y -= 0.03f;
	}
	if (input_->PushKey(DIK_LEFT)) {
		reticleTransform_.translate.x -= 0.03f;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		reticleTransform_.translate.x += 0.03f;
	}
}

void Player::BulletMove() {
	if (isSee) {
		bulletTransform_.translate = Add(bulletTransform_.translate,velocity);
		bu.translate = Add(bulletTransform_.translate,velocity);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE) && bulletCoolTime <= 0) {
		isSee = true;
		//bullet_->SetIsAlive(true);
		bulletCoolTime = kBulletMaxCoolTime;
		velocity = Subtract(reticleTransform_.translate,transform_.translate);
		velocity = Normalize(velocity);
		bulletTransform_.translate = transform_.translate;
		bu.translate = transform_.translate;
		//bullet_->Initialize(directX12_, "ghostPori", transform_.translate, velocity);
		//bullets_.push_back(newBullet);
	}
	if (bulletCoolTime > 0) {
		bulletCoolTime--;
	}
}

void Player::Draw() {
	model_->Draw();
	reticleModel_->Draw();
	//bulletModel_->Draw();
	bullet->Draw();
	/*if (bullet_->GetIsAlive()) {
		bullet_->Draw();
	}*/
}