#include "Player.h"

void Player::Initialize(const std::string& filename) {
	input_ = Input::GetInstance();

	model_ = std::make_unique<Model>();
	reticleModel_ = std::make_unique<Model>();
	bulletModel_ = std::make_unique<Model>();

	model_->Initialize(filename);
	reticleModel_->Initialize(filename);
	bulletModel_->Initialize(filename);

	transform_.translate = { 0.0f,0.0f,0.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.scale = { 0.3f,0.3f,0.3f };
	
	reticleTransform_.translate = { 0.0f,0.0f,25.0f };
	reticleTransform_.rotate = { 0.0f,0.0f,0.0f };
	reticleTransform_.scale = { 0.5f,0.5f,0.5f };

	bulletTransform_.translate = { -10.0f,0.0f,0.0f };
	bulletTransform_.rotate = { 0.0f,0.0f,0.0f };
	bulletTransform_.scale = { 0.3f,0.3f,0.3f };

	bulletCoolTime = 30;
}

void Player::Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight) {

	Move();
	ReticleMove();
	Attack();
	BulletMove();

	model_->Update(color, transform_,cameraTransform, directionalLight);
	reticleModel_->Update(color, reticleTransform_,cameraTransform, directionalLight);
	bulletModel_->Update(color, bulletTransform_,cameraTransform, directionalLight);

	for (Bullet* bullet : bullets) {
		bullet->Update(color, cameraTransform, directionalLight);
	}
}

void Player::Move() {
	if (input_->PushKey(DIK_W)) {
		transform_.translate.y += speed/2;
	}
	if (input_->PushKey(DIK_S)) {
		transform_.translate.y -= speed/2;
	}
	if (input_->PushKey(DIK_A)) {
		transform_.translate.x -= speed/2;
	}
	if (input_->PushKey(DIK_D)) {
		transform_.translate.x += speed/2;
	}
}

void Player::ReticleMove() {
	if (input_->PushKey(DIK_UP)) {
		reticleTransform_.translate.y += speed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		reticleTransform_.translate.y -= speed;
	}
	if (input_->PushKey(DIK_LEFT)) {
		reticleTransform_.translate.x -= speed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		reticleTransform_.translate.x += speed;
	}
}

void Player::BulletMove() {
	if (isSee) {
		bulletTransform_.translate = Add(bulletTransform_.translate,velocity);
	}
}

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE) && bulletCoolTime <= 0) {
		isSee = true;

		bulletCoolTime = kBulletMaxCoolTime;
		velocity = Subtract(reticleTransform_.translate,transform_.translate);
		velocity = Normalize(velocity);
		bulletTransform_.translate = transform_.translate;

		Bullet* newBullet = new Bullet();
		newBullet->Initialize(transform_.translate, velocity);
		newBullet->SetIsAlive(true);
		bullets.push_back(newBullet);
	}
	if (bulletCoolTime > 0) {
		bulletCoolTime--;
	}
}

void Player::Draw() {
	model_->Draw();
	reticleModel_->Draw();
	for (Bullet* bullet : bullets) {
		bullet->Draw();
	}
	//bulletModel_->Draw();
}