#include "Trace.h"
#include "../../../Object/Map/Map.h"
#include "../../../Entity/Player/Player.h"

void Trace::Initialize(Transform transform) {
	isChase = false;
	isAlive = true;
	isAttacking = false;

	ModelManager::GetInstance()->LoadModel("Entity/Enemy/trace");
	model.Initialize("Entity/Enemy/trace");
	model.transform = transform;
}

void Trace::Update() {
	FindPlayer();
	ChasePlayer();

	model.Update();
}
void Trace::Draw() {
	model.Draw();
}

void Trace::FindPlayer() {
	if (!isChase) {//未発見時
		Segment PtoE;
		PtoE.start = player->model.transform.translate;
		PtoE.end = model.transform.translate;

		int numWallCollision = 0;

		for (Wall wall : walls) {
			AABB wallAABB;
			wallAABB.CreateModelAABB(wall.model.transform);

			//視線と壁の当たり判定
			if (wallAABB.CheckLineCollision(PtoE)) {
				numWallCollision++;
			}
		}

		if (numWallCollision == 0) {
			isChase = true;
		}
	}
}
void Trace::ChasePlayer(){
	if (isChase) {

		if (!isAttacking) { //追跡中
			isAttacking = true;
			tracingTime = 0;

			distance = player->model.transform.translate - model.transform.translate;
			distance.y = 0.0f;

			chaseSpeed = firstSpeed;
		}
		else { //非追跡中
			tracingTime++;
			velocity = Normalize(distance) * chaseSpeed;

			chaseSpeed += 0.0005f;
			if (chaseSpeed >= maxSpeed) {
				chaseSpeed = maxSpeed;
			}

			model.transform.translate += velocity;

			// 最後に当たり判定をチェック
			Vector3 fixVector{};
			Vector3 zeroVector{};
			AABB enemyAABB;
			enemyAABB.CreateEntityAABB(model.transform);

			map->CheckCollision(enemyAABB, { velocity.x,velocity.z }, &fixVector);
			model.transform.translate += fixVector;

			//壁と当たったら
			if (fixVector.x !=0.0f || fixVector.z != 0.0f) {
				velocity = {};
				isAttacking = false;
				isChase = false;
			}

			//Playerと当たったら
		/*	if (enemyAABB.CheckCollision(player->GetCollision())) {
				velocity = {};
				isAttacking = false;
				isChase = false;
			}*/

			//時間経過
			if (tracingTime >= 240) {
				velocity = {};
				isAttacking = false;
				isChase = false;
			}
		}
	}
}