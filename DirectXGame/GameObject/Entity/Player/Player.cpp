#include "Player.h"

void Player::Initialize() {
	ModelManager::GetInstance()->LoadModel("player");
	model.Initialize("player");
	model.transform.translate = { 24.0f,1.5f,-48.0f };

	view.Initialize("player");
	view.transform.scale *= 0.25f;

	sLight.Initialize();

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();
}

void Player::Update() {
	LightUpdate();
	Move();

	CheckItemCollision();
	CheckItemBring();

	model.Update();
	view.Update();
	view.transform.translate = GetFrontVector(2.0f);
}

void Player::LightUpdate() {
	sLight.light->position = model.transform.translate;
	sLight.light->direction = GetFrontLightVector(1.0f);
	sLight.Update();
}

void Player::Draw() {
	//model.Draw();
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
		0,
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
	map_->CheckCollision(GetCollision(), { move.x,move.z }, &fixVector);
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

	Vector3 aa = map->GetItem()->model.transform.translate;
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