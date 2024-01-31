#include "Player.h"

void Player::Initialize() {
	input_ = Input::GetInstance();

	reticleModel_ = std::make_unique<Model>();

	reticleModel_->Initialize("reticle");

	body = std::make_unique<Model>();
	head = std::make_unique<Model>();
	left = std::make_unique<Model>();
	right = std::make_unique<Model>();

	body->Initialize("body");
	head->Initialize("head");
	left->Initialize("left");
	right->Initialize("right");


	transform_.translate = { 0.0f,0.0f,0.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.scale = { 0.1f,0.1f,0.1f };

	armTransform_ = transform_;
	
	reticleTransform_.translate = { 0.0f,0.0f,25.0f };
	reticleTransform_.rotate = { 0.0f,0.0f,0.0f };
	reticleTransform_.scale = { 0.25f,0.25f,0.25f };

	bulletCoolTime = 30;
}

void Player::Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight) {

	Move();
	ReticleMove();
	Attack();

	reticleModel_->Update(color, reticleTransform_,cameraTransform, directionalLight);

	transform_.rotate.y += 0.02f;
	
	armTransform_.translate = transform_.translate;
	armTransform_.rotate.x += 0.02f;

	Vector4 black = { 0,0,0,0 };
	body->Update(black, transform_, cameraTransform, directionalLight);
	head->Update(black, transform_, cameraTransform, directionalLight);
	left->Update(black, armTransform_, cameraTransform, directionalLight);
	right->Update(black, armTransform_, cameraTransform, directionalLight);


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

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE) && bulletCoolTime <= 0) {
		isSee = true;

		bulletCoolTime = kBulletMaxCoolTime;
		velocity = Subtract(reticleTransform_.translate,transform_.translate);
		velocity = Normalize(velocity);

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
	reticleModel_->Draw();

	body->Draw();
	head->Draw();
	left->Draw();
	right->Draw();

	for (Bullet* bullet : bullets) {
		bullet->Draw();
	}
}