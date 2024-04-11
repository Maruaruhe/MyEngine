#include "Player.h"
#include "../../../Manager/ModelManager.h"
#include "../../../Base/Input/Input.h"

Player::~Player(){
	 for (Bullet* bullet : bullets) {
		delete bullet;
	}
}

void Player::Initialize(const std::string& filename) {
	ModelManager::GetInstance()->LoadModel(filename);
	model = std::make_unique<Model>();
	model->Initialize(filename);

	reticle = std::make_unique<Reticle>();
	reticle->Initialize();
}

void Player::Update() {
	camera = model->GetCamera();
	reticle->model->SetCamera(camera);

	bullets.remove_if([](Bullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
   			return true;
		}
		return false;
		});


	Move();
	Shoot();


	model->Update();
	reticle->Update();

	for (Bullet* bullet : bullets) {
		bullet->Update();
	}

	ImGui::Begin("Player");
	ImGui::DragFloat3("pos", &model->transform.translate.x);
	ImGui::Text("bullets %d", bullets.size());
	for (Bullet* bullet : bullets) {
		ImGui::DragFloat3("bPos", &bullet->model.transform.translate.x);
	}
	ImGui::End();
}

void Player::Draw() {
	model->Draw();
	reticle->Draw();

	for (Bullet* bullet : bullets) {
		bullet->Draw();
	}
}

void Player::Move() {
	if (Input::GetInstance()->PushKey(DIK_W)) {
		model->transform.translate.y += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		model->transform.translate.y -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		model->transform.translate.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		model->transform.translate.x += 0.1f;
	}
}

void Player::Shoot() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = reticle->model->transform.translate - model->transform.translate;
		velocity = Normalize(velocity) / 3.0f;
		Bullet* newBullet = new Bullet();
		newBullet->Initialize(model->transform.translate, velocity,model->GetCamera());
		bullets.push_back(newBullet);
	}
}

void Player::OnCollision() {
//	isDead = true;
}