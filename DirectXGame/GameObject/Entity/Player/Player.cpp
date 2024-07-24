#include "Player.h"

void Player::Initialize() {
	ModelManager::GetInstance()->LoadModel("player");
	model.Initialize("player");
	model.transform.translate = { 19.0f,1.5f,-19.0f };

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();
}

void Player::Update() {
	Move();

	model.Update();

}

void Player::Draw() {
	//model.Draw();
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
	if (kInput->PushKey(DIK_Q)) {
		move.y += 0.1f;
	}
	if (kInput->PushKey(DIK_E)) {
		move.y -= 0.1f;
	}

	model.transform.translate += move;

	// 最後に当たり判定をチェック
	Vector3 fixVector{};
	map->CheckCollision(GetCollision(), { move.x,move.z }, &fixVector);
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
	lineOfSight.end = model.transform.translate + GetFrontVector() * dis;
	lineOfSight.end.y = 2.0f;
}

//向いている方向へのベクトルを求める関数
Vector3 Player::GetFrontVector() {
	Matrix4x4 wM = MakeRotateXYZMatrix(model.transform.rotate);
	Vector3 dir = { 0.0f,0.0f,1.0f };
	return Normalize(vecMat(dir, wM));
}