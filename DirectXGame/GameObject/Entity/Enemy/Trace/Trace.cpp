#include "Trace.h"
#include "../../../Object/Map/Map.h"
#include "../../../Entity/Player/Player.h"

void Trace::Initialize(Transform transform) {
	isChase_ = false;
	isAlive_ = true;
	isAttacking_ = false;

	ModelManager::GetInstance()->LoadModel("Entity/Enemy/trace");
	model_.Initialize("Entity/Enemy/trace");
	model_.transform = transform;
}

void Trace::Update() {
	FindPlayer();
	ChasePlayer();

	model_.Update();
}
void Trace::Draw() {
	model_.Draw();
}

void Trace::FindPlayer() {
	if (!isChase_) {//未発見時
		Segment PtoE;
		PtoE.start = player_->model.transform.translate;
		PtoE.end = model_.transform.translate;

		int numWallCollision = 0;

		for (Wall wall : walls_) {
			AABB wallAABB;
			wallAABB.CreateModelAABB(wall.model_.transform);

			//視線と壁の当たり判定
			if (wallAABB.CheckLineCollision(PtoE)) {
				numWallCollision++;
			}
		}

		if (numWallCollision == 0) {
			isChase_ = true;
		}
	}
}
void Trace::ChasePlayer(){
	if (isChase_) {

		if (!isAttacking_) { //追跡中
			isAttacking_ = true;
			tracingTime_ = 0;

			distance_ = player_->model.transform.translate - model_.transform.translate;
			distance_.y = 0.0f;

			chaseSpeed_ = firstSpeed_;
		}
		else { //非追跡中
			tracingTime_++;
			velocity_ = Normalize(distance_) * chaseSpeed_;

			chaseSpeed_ += 0.0005f;
			if (chaseSpeed_ >= maxSpeed_) {
				chaseSpeed_ = maxSpeed_;
			}

			model_.transform.translate += velocity_;

			// 最後に当たり判定をチェック
			Vector3 fixVector{};
			Vector3 zeroVector{};
			AABB enemyAABB;
			enemyAABB.CreateEntityAABB(model_.transform);

			map_->CheckCollision(enemyAABB, { velocity_.x,velocity_.z }, &fixVector);
			model_.transform.translate += fixVector;

			//壁と当たったら
			if (fixVector.x !=0.0f || fixVector.z != 0.0f) {
				velocity_ = {};
				isAttacking_ = false;
				isChase_ = false;
			}

			//Playerと当たったら
		/*	if (enemyAABB.CheckCollision(player->GetCollision())) {
				velocity = {};
				isAttacking = false;
				isChase = false;
			}*/

			//時間経過
			if (tracingTime_ >= 240) {
				velocity_ = {};
				isAttacking_ = false;
				isChase_ = false;
			}
		}
	}
}