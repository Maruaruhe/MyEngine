#include "Trace.h"
#include "../../../Object/Map/MapJson.h"
#include "../../../Entity/Player/Player.h"

using namespace MyEngine;

void Trace::Initialize(Transform transform) {
	isChase_ = false;
	isAlive_ = true;
	isAttacking_ = false;

	ModelManager::GetInstance()->LoadModel("Entity/Enemy/trace");
	model_.Initialize("Entity/Enemy/trace");
	model_.transform = transform;

	//model_.transform.translate = {0.0f,0.0f,0.0f};
	model_.transform.scale = {1.0f,1.0f,1.0f};
	model_.transform.rotate = {0.0f,0.0f,0.0f};
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
	//壁カウント用変数
		int numWallCollision = 0;

	if (!isChase_) {//未発見時
		Segment PtoE;
		PtoE.start = player_->model.transform.translate;
		PtoE.end = model_.transform.translate;
		PtoE.end.y = 1.0f;

		walls_ = map_->GetWalls();
		for (Wall wall : walls_) {
			AABB wallAABB;
			wallAABB.CreateWallAABB(wall.model_.transform);
			//視線と壁の当たり判定
			if (wallAABB.CheckLineCollision(PtoE)) {
				numWallCollision++;
			}
		}
		//視線上に壁がなかった場合発見
		if (numWallCollision == 0) {
			isChase_ = true;
		}

	}
#ifdef _DEBUG
		ImGui::Begin("Trace");
		ImGui::SliderInt("numWall", &numWallCollision, -15, 15);
		ImGui::End();
#endif

}
void Trace::ChasePlayer(){
	if (isChase_) {//発見

		if (!isAttacking_) { //攻撃開始時初期化
			isAttacking_ = true;
			tracingTime_ = 0;

			distance_ = player_->model.transform.translate - model_.transform.translate;
			distance_.y = 0.0f;

			chaseSpeed_ = firstSpeed_;
		}
		else { //追跡中
			tracingTime_++; //追跡時間
			velocity_ = Normalize(distance_) * chaseSpeed_;

			chaseSpeed_ += 0.0005f; // だんだんスピードはやく
			if (chaseSpeed_ >= maxSpeed_) {
				chaseSpeed_ = maxSpeed_;
			}

			model_.transform.translate += velocity_;
			model_.transform.translate.y = 0.0f;

			// 最後に当たり判定をチェック
			Vector3 fixVector{};
			AABB enemyAABB;
			enemyAABB.CreateEntityAABB(model_.transform);

			map_->CheckCollision(enemyAABB, { velocity_.x,velocity_.y, velocity_.z }, &fixVector);
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