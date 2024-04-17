#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	player = std::make_unique<Player>();
	player->Initialize("ghostsuperkyon");
	player->model->transform.translate.z = 15.0f;
	player->model->SetCamera(camera2);
	player->model->isParent = true;

	enemy = std::make_unique<Enemy>();
	enemy->Initialize("axis",player.get(), {0,0,20});
	enemy->model->SetCamera(camera2);
	enemy->ResetTime();

	skydome = std::make_unique<Skydome>();
	skydome->Initialize("skydome");
	skydome->model->SetCamera(camera2);

	sprite = std::make_unique<Sprite>();
	sprite->Initialize({ 0,0 }, {320,180}, "Resources/ao.png");
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	player->Update();
	enemy->Update();
	skydome->Update();
	sprite->Update();

	CheckAllCollision();
}

void TestScene::Draw() {

	player->Draw();
	enemy->Draw();
	skydome->Draw();
	sprite->Draw();
}

void TestScene::CheckAllCollision() {
	Vector3 posA, posB;

	const std::list<Bullet*>& playerBullets_ = player->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets_ = enemy->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	posA = player->model->transform.translate;

	for (EnemyBullet* bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();

		Vector3 distance = Subtract(posA, posB);
		if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <= 2 * 2) {
			player->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
		posA = enemy->GetWorldPosition();

		for (Bullet* bullet : playerBullets_) {
			posB = bullet->GetWorldPosition();

			Vector3 distance = Subtract(posA, posB);
			if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <=
				2 * 2) {
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (Bullet* playerBullet : playerBullets_) {
		for (EnemyBullet* eBullet : enemyBullets_) {
			posA = playerBullet->GetWorldPosition();
			posB = eBullet->GetWorldPosition();

			Vector3 distance = Subtract(posA, posB);
			if (std::pow(distance.x, 2) + std::pow(distance.y, 2) + std::pow(distance.z, 2) <=
				2 * 2) {
				playerBullet->OnCollision();
				eBullet->OnCollision();
			}
		}
	}
#pragma endregion
}