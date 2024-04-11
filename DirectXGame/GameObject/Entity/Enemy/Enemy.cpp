#include "Enemy.h"
#include <assert.h>
#include "../../../Manager/ModelManager.h"

#include	"../Player/Player.h"

Enemy::Enemy() {
	state = new EnemyStateApproach();
}

Enemy::~Enemy() {
}

void Enemy::Initialize(const std::string& filename, Player* player, const Vector3& pos) {
	ModelManager::GetInstance()->LoadModel(filename);
	model = std::make_unique<Model>();
	model->Initialize(filename);

	model->transform.translate = pos;

	isDead = false;

	this->player = player;
}

void Enemy::Update() {
	if (!isDead) {
		state->Update(this);
		model->Update();
	}

	bullets.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

		for (EnemyBullet* bullet : bullets) {
			bullet->Update();
		}
		for (TimedCall* timedCall : timedCalls) {
			timedCall->Update();
		}

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("pos", &model->transform.translate.x);
	ImGui::Text("bullets %d", bullets.size());
	ImGui::End();
}

void Enemy::Draw() {
	if (!isDead) {
		model->Draw();
	}
	for (EnemyBullet* bullet : bullets) {
		bullet->Draw();
	}
}

void Enemy::move(const Vector3& velocitya) {
	model->transform.translate += velocitya;
}

void Enemy::Fire() {
	if (!isDead) {
		Vector3 distance = Subtract(model->transform.translate, player->model->transform.translate);
		Vector3 normalize = Normalize(distance);
		Vector3 velocity = {};
		velocity = -normalize / 3.0f;

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model->transform.translate, velocity,model->GetCamera());

		bullets.push_back(newBullet);
	}
}

void Enemy::ResetTime() {
	Fire();

	std::function<void(void)> callback = std::bind(&Enemy::ResetTime, this);
	TimedCall* timedCall = new TimedCall(callback, 60);
	timedCalls.push_back(timedCall);
}

void Enemy::ChangeState(BaseEnemyState* newState) {
	state = newState;
}

void EnemyStateApproach::Update(Enemy* enemy) {
	const Vector3 velocity = { 0.0f, 0.0f, -0.03f };
	enemy->move(velocity);
	if (enemy->model->transform.translate.z < 0.0f) {
		enemy->ChangeState(new EnemyStateLeave());
	}
}
void EnemyStateLeave::Update(Enemy* enemy) {
	const Vector3 velocity = { -0.25f, 0.25f, -0.25f };
	enemy->move(velocity);
}

void Enemy::OnCollision() {
//	isDead = true;
}