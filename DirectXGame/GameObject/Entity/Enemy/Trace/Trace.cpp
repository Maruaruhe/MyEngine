#include "Trace.h"
#include "../../../Object/Map/Map.h"
#include "../../../Entity/Player/Player.h"

void Trace::Initialize(Transform transform, Camera* camera, Map* map, Player* player) {
	isChase = false;
	isAlive = true;
	isAttacking = false;

	ModelManager::GetInstance()->LoadModel("Entity/Enemy/trace");
	model.Initialize("Entity/Enemy/trace");
	model.transform = transform;
	model.SetCamera(camera);

	this->map =map;
	this->player = player;

	walls = map->GetWall();
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

		if (!isAttacking) {
			isAttacking = true;
			tracingTime = 0;

			Vector3 distance;
			distance = player->model.transform.translate - model.transform.translate;
			distance.y = 0.0f;

			float setSpeed = 0.05f;
			velocity = Normalize(distance) * setSpeed;
		}
		else {
			tracingTime++;
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
			if (tracingTime >= 300) {
				velocity = {};
				isAttacking = false;
				isChase = false;
			}
		}
	}
}