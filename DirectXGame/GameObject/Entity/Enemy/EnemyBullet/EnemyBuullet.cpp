#include "EnemyBuullet.h"
#include <assert.h>

#include "../../../../Manager/ModelManager.h"

void EnemyBullet::Initialize(const Vector3& pos, const Vector3& velo, Camera* camera) {
	ModelManager::GetInstance()->LoadModel("bullet");
	model.Initialize("bullet");

	model.SetCamera(camera);

	model.transform.scale = { 0.3f,0.3f,2.0f };

	model.transform.translate = pos;
	model.transform.rotate.y = std::atan2(velo.x, velo.z);
	double dis = std::sqrt(pow(velo.x, 2) + pow(velo.z, 2));
	model.transform.rotate.x = std::atan2(-velo.y, float(dis));
	velocity = velo;
}

void EnemyBullet::Update() {
	model.transform.translate += velocity;
	if (--deathTimer <= 0) {
		isDead_ = true;
	}

	model.Update();
}

void EnemyBullet::Draw() {
	model.Draw();
}

void EnemyBullet::OnCollision() { isDead_ = true; }