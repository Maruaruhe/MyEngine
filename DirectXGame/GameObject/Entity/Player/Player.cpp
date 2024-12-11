#include "Player.h"

void Player::Initialize() {
	ModelManager::GetInstance()->LoadModel("player");
	ModelManager::GetInstance()->LoadModel("Entity/player/deadbody");
	model.Initialize("player");
	deadModel.Initialize("Entity/player/deadbody");
	
	//model.transform.translate = { 24.0f,1.5f,-48.0f };
	model.transform.translate = { 0.0f,0.0f,0.0f };
	model.transform.scale.y *= 2.0f;

	view.Initialize("player");
	view.transform.scale *= 0.25f;

	sLight.Initialize();

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();

	state_.HP;
	state_.isDash;
	state_.isJump = false;
	state_.isUsingLight = false;
	state_.isAlive = true;
	state_.kMapSpeed;
	state_.moveSpeed;
	state_.stamina;
	state_.weight;

	tForCamera = {};
	deadCamera = {};

	deadFlame = 0;
	aliveFrame = 0;

	deads.Initialize({ 1280,720 }, "Resources/Dead/youdied.png");
}

void Player::Update() {
	//aliveFrame++;
	if (aliveFrame >= 180) {
		state_.isAlive = false;
	}
	LightUpdate();
	if (state_.isAlive) {
		Move();
		Jump();
	}
	else {
	DeathUpdate();
	}


	CheckItemCollision();
	CheckItemBring();

	model.Update();
	view.Update();
	view.transform.translate = GetFrontVector(2.0f);

	if (kInput->TriggerKey(DIK_T)) {
		state_.isAlive = false; 
		deadFlame = 0;
	}
	if (kInput->TriggerKey(DIK_Y)) {
		state_.isAlive = true;
	}
	if (state_.isAlive) {
		tForCamera = model.transform;
	}
	else {
		tForCamera = deadCamera;
	}
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::SliderFloat3("translate", &model.transform.translate.x, -15, 15);
	ImGui::SliderFloat3("rotate", &model.transform.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("scale", &model.transform.scale.x, 1.0f, 10.0f);
	ImGui::End();
#endif
}

void Player::LightUpdate() {
	if (state_.isAlive) {
		if (kInput->TriggerKey(DIK_Q)) {
			if (state_.isUsingLight) {
				state_.isUsingLight = false;
				sLight.light->color = { 0.0f,0.0f,0.0f,0.0f };
			}
			else {
				state_.isUsingLight = true;
				sLight.light->color = { 1.0f,1.0f,1.0f,1.0f };
			}
		}

		sLight.light->position = model.transform.translate;
		sLight.light->direction = GetFrontLightVector(1.0f);
	}
	sLight.Update();
}

void Player::Draw() {
	model.Draw();
	if (!state_.isAlive) {
		deadModel.Draw();
		deads.Draw();
	}
#ifdef _DEBUG
	view.Draw();
#endif // DEBUG
}

void Player::Move() {
	Vector3 move{};

	if (kInput->PushKey(DIK_W)) {
		move.z += 0.1f;
	}
	if (kInput->PushKey(DIK_S)) {
		move.z -= 0.1f;
	}
	if (kInput->PushKey(DIK_A)) {
		move.x -= 0.1f;
	}

	if (kInput->PushKey(DIK_D)) {
		move.x += 0.1f;
	}

	//rotate
	Vector2 rotate{};

	if (kInput->PushKey(DIK_LEFT)) {
		rotate.y -= 0.03f;
	}
	if (kInput->PushKey(DIK_RIGHT)) {
		rotate.y += 0.03f;
	}
	if (kInput->PushKey(DIK_UP)) {
		rotate.x -= 0.03f;
	}
	if (kInput->PushKey(DIK_DOWN)) {
		rotate.x += 0.03f;
	}

	//transformにRotateを入れてく
	model.transform.rotate.x += rotate.x;
	model.transform.rotate.y += rotate.y;

	Vector3 noCountRotateX = model.transform.rotate;
	noCountRotateX.x = 0.0f;

	//向いてる方向へ移動するように
	Matrix4x4 wM = MakeRotateXYZMatrix(noCountRotateX);
	move = {
		move.x * wM.m[0][0] + move.z * wM.m[2][0],
		//move.x * wM.m[0][1] + move.y * wM.m[2][1],
		{},
		move.x * wM.m[0][2] + move.z * wM.m[2][2]
	};

	//y
#ifdef _DEBUG
	if (kInput->PushKey(DIK_Q)) {
		move.y += 0.1f;
	}
	if (kInput->PushKey(DIK_E)) {
		move.y -= 0.1f;
	}
#endif // DEBUG

	model.transform.translate += move;

	// 最後に当たり判定をチェック
	Vector3 fixVector{};
	//map_->CheckCollision(GetCollision(), { move.x, state_.velocity.y, move.z }, &fixVector);
	model.transform.translate += fixVector;

	Vector3 fixVec{};
	mapJson_->CheckCollision(GetCollision(), { move.x, state_.velocity.y, move.z }, & fixVec);
	model.transform.translate += fixVec;
}

void Player::Jump() {

	//Jump
	if (!state_.isJump) {
		if (kInput->PushKey(DIK_SPACE)) {
			state_.velocity.y = 0.2f;
			state_.isJump = true;
		}
	}
	else {
		//Jumping
		state_.velocity.y -= 0.02f;
	}
	model.transform.translate += state_.velocity;

	Vector3 fixVector{};

		//if (map_->CheckCollisionWithFloor(GetCollision(), state_.velocity, &fixVector)) {
	mapJson_->CheckCollisionFloor(GetCollision(), state_.velocity, &fixVector, &state_.isJump);
		if (!state_.isJump) {
			Vector3 a = fixVector;
			state_.isJump = false;
			state_.velocity.y = 0.0f;
		}
		else {
			state_.isJump = true;
		}

	model.transform.translate += fixVector;
}

AABB Player::GetCollision() {
	AABB a;
	a.CreateEntityAABB(model.transform);

	return a;
}

Vector3 Player::CheckLineOfSightCollision() {
	Segment lineOfSight;
	float dis = 100.0f;
	//Player視線(線分)
	lineOfSight.start = model.transform.translate;
	lineOfSight.start.y = 2.0f;
	lineOfSight.end = model.transform.translate + GetFrontVector(2.0f) * dis;
	lineOfSight.end.y = 2.0f;

	return {};
}

//向いている方向へのベクトルを求める関数
Vector3 Player::GetFrontVector(float length) {
	Matrix4x4 wM = MakeRotateXYZMatrix(model.transform.rotate);
	Vector3 dir = { 0.0f,0.0f,1.0f };
	Vector3 a= model.transform.translate + Normalize(vecMat(dir, wM)) * length;
	return a;
}

//向いている方向へのベクトルを求める関数
Vector3 Player::GetFrontLightVector(float length) {
	Matrix4x4 wM = MakeRotateXYZMatrix(model.transform.rotate);
	Vector3 dir = { 0.0f,0.0f,1.0f };
	Vector3 a= Normalize(vecMat(dir, wM)) * length;
	return a;
}

//向いている方向へのベクトルを求める関数
Vector3 Player::GetItemFrontVector() {
	Transform transform;
	transform = model.transform;
	transform.rotate.x = model.transform.rotate.x + 0.25f;
	transform.rotate.y = model.transform.rotate.y + 0.50f;

	Matrix4x4 wM = MakeRotateXYZMatrix(transform.rotate);
	Vector3 dir = { 0.0f,0.0f,1.0f };
	Vector3 a= model.transform.translate + Normalize(vecMat(dir, wM)) * 1.0f;
	return a;
}

void Player::CheckItemCollision() {
	AABB itemAABB;
	itemAABB.CreateEntityAABB(map_->GetItem()->model.transform);

	Segment playerSight;
	playerSight.start = model.transform.translate;
	playerSight.end = GetFrontVector(2.0f);

	int count = 0;
	if (itemAABB.CheckLineCollision(playerSight)) {
		count += 1;
		map_->GetItem()->isabletobetaken = true;
		map_->GetItem()->TakenItem();
	}
	else {
		map_->GetItem()->isabletobetaken = false;
	}

#ifdef _DEBUG

	ImGui::Begin("Item");

	Vector3 aa = map_->GetItem()->model.transform.translate;
	ImGui::DragFloat3("pos", &aa.x);
	ImGui::DragFloat3("sight.start", &playerSight.start.x);
	ImGui::DragFloat3("sight.end", &playerSight.end.x);
	ImGui::DragInt("count", &count);

	ImGui::End();

#endif // DEBUG
}


void Player::CheckItemBring() {
	if (map_->GetItem()->isTaken) {
		//所持しているとき手に持つ
		map_->GetItem()->model.transform.translate = GetItemFrontVector();
		//PlayerのRotateと同期
		map_->GetItem()->model.transform.rotate = model.transform.rotate;

		//
		if (KeyInput::GetInstance()->PushKey(DIK_G)) {//Drop処理
			map_->GetItem()->isTaken = false;
			//足元に落とす & リセット
			map_->GetItem()->model.transform.translate = GetFrontVector(0.7f);
			map_->GetItem()->model.transform.translate.y = 0.0f;
			map_->GetItem()->model.transform.scale = { 1.0f,1.0f,1.0f };
			map_->GetItem()->model.transform.rotate.x = {};
			map_->GetItem()->model.transform.rotate.z = {};
		}
	}
}

void Player::DeathUpdate() {
	if (!state_.isAlive) {
		deadFlame++;
		if (deadFlame >= 180) {
			deadFlame = 180;
		}

		deadCamera.translate = model.transform.translate;
		deadCamera.translate.y = 4.0f + float(3 * deadFlame)/180.0f;
		deadCamera.rotate.x = 1.27f;

		deadModel.transform.translate = model.transform.translate;
		deadModel.transform.rotate.y = 1.77f;

		deads.materialData_->color.w = float(1+deadFlame)/180.0f;
	}

#ifdef _DEBUG

	ImGui::Begin("DeathCamera");
	ImGui::SliderFloat3("translate", &deadCamera.translate.x, -15, 15);
	ImGui::SliderFloat3("rotate", &deadCamera.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("scale", &deadCamera.scale.x, 1.0f, 10.0f);

	ImGui::SliderFloat3("deadModeltranslate", &deadModel.transform.translate.x, -15, 15);
	ImGui::SliderFloat3("deadModelrotate", &deadModel.transform.rotate.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("deadModelscale", &deadModel.transform.scale.x, 1.0f, 10.0f);
	ImGui::End();
#endif // DEBUG_
}