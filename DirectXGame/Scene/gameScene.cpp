#include "GameScene.h"

void GameScene::Initialize()
{
	input = Input::GetInstance();
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;

	colorVolume[0] = 1.0f;
	colorVolume[1] = 1.0f;
	colorVolume[2] = 1.0f;

	player_ = std::make_unique<Player>();
	enemy_ = std::make_unique<Enemy>();
	camera = std::make_unique<Camera>();

	player_->Initialize("ghostPori");
	enemy_->Initialize("ghostPori");
	camera->Initialize();
}

void GameScene::Update() {

	Vector4 color = {colorVolume[0],colorVolume[1],colorVolume[2],1.0f};

	player_->Update(color, camera->GetTransform(), light);
	Vector4 eColor = { 0.0f,0.0f,0.0f,1.0f };
	enemy_->Update(eColor, camera->GetTransform(), light);
	if (isCollsion()) {
		sceneNo = CLEAR;
	}
}

bool GameScene::isCollsion() {
//	std::list<Bullet*> bullets = player_->GetBullets();
//	for (Bullet* bullet : bullets) {
//		Vector3 pPos = bullet->GetPosition();
//		Vector3 ePos = enemy_->GetTransform().translate;
//		Vector3 dis = Subtract(pPos, ePos);
//
//		if ((dis.x * dis.x + dis.y * dis.y + dis.z * dis.z) <= 2) {
//			enemy_->SetIsAlive(false);
//			return true;
//		}
//		return false;
//	}
//}
	Vector3 pPos = player_->GetBulletTransform().translate;
	Vector3 ePos = enemy_->GetTransform().translate;
	Vector3 dis = Subtract(pPos, ePos);
	if ((dis.x * dis.x + dis.y * dis.y + dis.z * dis.z) <= 2) {
		enemy_->SetIsAlive(false);
		return true;
	}
	return false;
}


void GameScene::Draw() {
	player_->Draw();
	enemy_->Draw();
}