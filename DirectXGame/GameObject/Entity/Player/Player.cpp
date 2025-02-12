#include "Player.h"

using namespace MyEngine;

void Player::Initialize() {
	ModelManager::GetInstance()->LoadModel("player");
	ModelManager::GetInstance()->LoadModel("Entity/player/deadbody");
	model.Initialize("player");
	deadModel.Initialize("Entity/player/deadbody");
	
	//model.transform.translate = { 24.0f,1.5f,-48.0f };
	model.transform.translate = { 0.0f,0.0f,0.0f };
	model.transform.scale.x *= 0.5f;
	model.transform.scale.z *= 0.5f;
	model.transform.scale.y *= 2.0f;

	view.Initialize("player");
	view.transform.scale *= 0.25f;

	sLight_.Initialize();

	kInput_ = KeyInput::GetInstance();
	pInput_ = GamePadInput::GetInstance();

	//State Initialize
	state_.HP;
	state_.isDash;
	state_.isJump = false;
	state_.onFloor = false;
	state_.isUsingLight = false;
	state_.isAlive = true;
	state_.kMapSpeed;
	state_.moveSpeed;
	state_.stamina;
	state_.weight;
	for (int i = 0; i < 4; i++) {
		state_.itemSlot[i].isFilled = false;
		state_.itemSlot[i].isSelected = false;
		state_.itemSlot[i].sprite[0].Initialize({64,64}, "Resources/Entity/Player/itemEmptySlot.png");
		state_.itemSlot[i].sprite[1].Initialize({64,64}, "Resources/Entity/Player/itemFillSlot.png");

		int distance = 120;
		state_.itemSlot[i].sprite[0].transform.translate = {460.0f + distance * i,  600, 0};
		state_.itemSlot[i].sprite[1].transform.translate = {460.0f + distance * i,  600, 0};
	}
	state_.itemSlot[0].isSelected = true;

	tForCamera = {};
	deadCamera = {};

	deadFlame = 0;
	aliveFrame = 0;

	canOpenDoor_ = false;

	//テクスチャ
	deads_.Initialize({ 1280,720 }, "Resources/Dead/youdied.png");
	deads_.anchorPoint = {};
	toggleLight_.Initialize({ 214,31 }, "Resources/Entity/Player/ToggleLight.png");
	toggleLight_.transform.translate = { 1100,30,0 };

	dropItem_.Initialize({ 200,35 }, "Resources/Entity/Player/dropItem.png");
	dropItem_.anchorPoint = { 0.5f,0.5f };
	dropItem_.transform.translate = { 1100,70,0 };

	holdQ_.Initialize({ 358,31 }, "Resources/Entity/Player/HoldR.png");
	holdQ_.anchorPoint = { 0.5f,0.5f };
	holdQ_.transform.translate = { 640,360,0 };

	takeItem_.Initialize({ 200,30 }, "Resources/Entity/Player/takeItem.png");
	takeItem_.anchorPoint = { 0.5f,0.5f };
	takeItem_.transform.translate = { 640,360,0 };


	wasd_.w[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseW.png");
	wasd_.w[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueW.png");
	wasd_.w[0].transform.translate = { 42 + kXkeys,10 + kYkeys,0 };
	wasd_.w[1].transform.translate = { 42 + kXkeys,10 + kYkeys,0 };

	wasd_.a[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseA.png");
	wasd_.a[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueA.png");
	wasd_.a[0].transform.translate = { 10 + kXkeys,42 + kYkeys,0 };
	wasd_.a[1].transform.translate = { 10 + kXkeys,42 + kYkeys,0 };

	wasd_.s[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseS.png");
	wasd_.s[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueS.png");
	wasd_.s[0].transform.translate = { 42 + kXkeys,42 + kYkeys,0 };
	wasd_.s[1].transform.translate = { 42 + kXkeys,42 + kYkeys,0 };

	wasd_.d[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseD.png");
	wasd_.d[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueD.png");
	wasd_.d[0].transform.translate = { 74 + kXkeys,42 + kYkeys,0 };
	wasd_.d[1].transform.translate = { 74 + kXkeys,42 + kYkeys,0 };

	//Arrow-------------------------------------------------------------------------------------------------------

	arrows_.w[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseUp.png");
	arrows_.w[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueUp.png");
	arrows_.w[0].transform.translate = { 42 + kXAkeys,10 + kYAkeys,0 };
	arrows_.w[1].transform.translate = { 42 + kXAkeys,10 + kYAkeys,0 };

	arrows_.a[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseLeft.png");
	arrows_.a[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueLeft.png");
	arrows_.a[0].transform.translate = { 10 + kXAkeys,42 + kYAkeys,0 };
	arrows_.a[1].transform.translate = { 10 + kXAkeys,42 + kYAkeys,0 };

	arrows_.s[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseDown.png");
	arrows_.s[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueDown.png");
	arrows_.s[0].transform.translate = { 42 + kXAkeys,42 + kYAkeys,0 };
	arrows_.s[1].transform.translate = { 42 + kXAkeys,42 + kYAkeys,0 };

	arrows_.d[0].Initialize({ 32,32 }, "Resources/Entity/Player/falseRight.png");
	arrows_.d[1].Initialize({ 32,32 }, "Resources/Entity/Player/trueRight.png");
	arrows_.d[0].transform.translate = { 74 + kXAkeys,42 + kYAkeys,0 };
	arrows_.d[1].transform.translate = { 74 + kXAkeys,42 + kYAkeys,0 };

	space_.sprite[0].Initialize({ 64,32 }, "Resources/Entity/Player/falseSpace.png");
	space_.sprite[1].Initialize({ 64,32 }, "Resources/Entity/Player/trueSpace.png");
	space_.sprite[0].transform.translate = {640,680,0};
	space_.sprite[1].transform.translate = { 640,680,0 };
	space_.sprite[0].transform.scale*=2;
	space_.sprite[1].transform.scale *= 2;
	space_.isUsed = false;
}

void Player::Update() {
	//aliveFrame++;
	if (aliveFrame >= 180) {
		state_.isAlive = false;
	}
	LightUpdate();

	//移動関連
	if (state_.isAlive) {
		Move();
		Jump();
		ItemSlotUpdate();
	}
	else {
	DeathUpdate();
	deadModel.transform = model.transform;
	}


	CheckItemCollision();
	//CheckItemBring();

	model.Update();
	view.Update();
	view.transform.translate = GetFrontVector(2.0f);

	//テクスチャ
	toggleLight_.Update();
	holdQ_.Update();
	takeItem_.Update();
	dropItem_.Update();

	for (int i = 0; i < 2; i++) {
		wasd_.w[i].Update();
		wasd_.a[i].Update();
		wasd_.s[i].Update();
		wasd_.d[i].Update();

		arrows_.w[i].Update();
		arrows_.a[i].Update();
		arrows_.s[i].Update();
		arrows_.d[i].Update();

		space_.sprite[i].Update();
	}


	if (kInput_->TriggerKey(DIK_T)) {
		state_.isAlive = false; 
		deadFlame = 0;
	}
	if (kInput_->TriggerKey(DIK_Y)) {
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
	ImGui::Text("%d", state_.onFloor);
	ImGui::End();
#endif
}

void Player::LightUpdate() {
	if (state_.isAlive) {
		//ON
		if (kInput_->TriggerKey(DIK_Q)) {
			if (state_.isUsingLight) {
				state_.isUsingLight = false;
				sLight_.light_->color = { 0.0f,0.0f,0.0f,0.0f };
			}
			//OFF
			else {
				state_.isUsingLight = true;
				sLight_.light_->color = { 1.0f,1.0f,1.0f,1.0f };
			}
		}

		sLight_.light_->position = model.transform.translate;
		sLight_.light_->direction = GetFrontLightVector(1.0f);
	}
	sLight_.Update();
}

void Player::Draw() {
	//model.Draw();

	if (!state_.isAlive) {
		deadModel.Draw();
	}
#ifdef _DEBUG
	view.Draw();
#endif // DEBUG
}

void Player::SpriteDraw() {
	if (state_.isAlive) {
		toggleLight_.Draw();
		if (canDropItem_) {
			dropItem_.Draw();
		}

		if (canOpenDoor_) {
			holdQ_.Draw();
		}
		if (canTakeItem_) {
			takeItem_.Draw();
		}

		wasd_.w[wasd_.isw].Draw();
		wasd_.a[wasd_.isa].Draw();
		wasd_.s[wasd_.iss].Draw();
		wasd_.d[wasd_.isd].Draw();

		arrows_.w[arrows_.isw].Draw();
		arrows_.a[arrows_.isa].Draw();
		arrows_.s[arrows_.iss].Draw();
		arrows_.d[arrows_.isd].Draw();

		space_.sprite[space_.isUsed].Draw();

		for (int i = 0; i < 4; i++) {
			state_.itemSlot[i].sprite[state_.itemSlot[i].isFilled].Draw();
		}
	}

	if (!state_.isAlive) {
		deads_.Draw();
	}
}

void Player::Move() {
	Vector3 move{};

	float moveSpeed = 0.075f;
	//Dash
	if (KeyInput::GetInstance()->PushKey(DIK_LSHIFT)) {
		moveSpeed = 0.125f;
	}

	if (kInput_->PushKey(DIK_W)) {
		move.z += moveSpeed;
		wasd_.isw = true;
	}
	else {
		wasd_.isw = false;
	}
	if (kInput_->PushKey(DIK_S)) {
		move.z -= moveSpeed;
		wasd_.iss = true;
	}
	else {
		wasd_.iss = false;
	}
	if (kInput_->PushKey(DIK_A)) {
		move.x -= moveSpeed;
		wasd_.isa = true;
	}
	else {
		wasd_.isa = false;
	}

	if (kInput_->PushKey(DIK_D)) {
		move.x += moveSpeed;
		wasd_.isd = true;
	}
	else {
		wasd_.isd = false;
	}

	//斜め移動の場合　減速
	if (move.x != 0.0f && move.z != 0.0f) {
		move.x *= 0.71f;
		move.z *= 0.71f;
	}

	//rotate
	Vector2 rotate{};

	if (kInput_->PushKey(DIK_LEFT)) {
		rotate.y -= 0.03f;
		arrows_.isa = true;
	}
	else {
		arrows_.isa = false;
	}
	if (kInput_->PushKey(DIK_RIGHT)) {
		rotate.y += 0.03f;
		arrows_.isd = true;
	}
	else {
		arrows_.isd = false;
	}
	if (kInput_->PushKey(DIK_UP)) {
		rotate.x -= 0.03f;
		arrows_.isw = true;
	}
	else {
		arrows_.isw = false;
	}
	if (kInput_->PushKey(DIK_DOWN)) {
		rotate.x += 0.03f;
		arrows_.iss = true;
	}
	else {
		arrows_.iss = false;
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
	if (kInput_->PushKey(DIK_Q)) {
		move.y += 0.1f;
	}
	if (kInput_->PushKey(DIK_E)) {
		move.y -= 0.1f;
	}
#endif // DEBUG

	model.transform.translate += move;

	// 最後に当たり判定をチェック
	Vector3 fixVec{};
	mapJson_->CheckCollision(GetCollision(), { move.x, state_.velocity.y, move.z }, & fixVec);
	model.transform.translate += fixVec;

	//死亡チェック
	if (model.transform.translate.y <= -7.0f) {
		state_.isAlive = false;
	}
}

void Player::Jump() {

	//Jump
	if (kInput_->TriggerKey(DIK_SPACE)) {
		if (!state_.isJump) {
			state_.velocity.y = 0.25f;
			state_.isJump = true;
		}
		space_.isUsed = true;
	}
	else {
		space_.isUsed = false;
	}

	model.transform.translate += state_.velocity;

	Vector3 fixVector{};

	//床との当たり判定
	mapJson_->CheckCollisionFloor(GetCollision(), &state_.velocity, &fixVector, &state_.onFloor);

	if (!state_.onFloor) { //空中
		state_.velocity.y -= 0.02f;
	}
	else { //地上
		//state_.velocity.y = 0.0f;
		state_.isJump = false;
	}

	model.transform.translate += fixVector;
}

void Player::ItemSlotUpdate() {
	//現在のアイテムスロット番号を取得
	int whereNowSlot = GetNowSlot();
	//ローテーション
	if (KeyInput::GetInstance()->TriggerKey(DIK_O)) {
		whereNowSlot--;
		if (whereNowSlot <= -1) {
			whereNowSlot = 3;
		}
	}
	if (KeyInput::GetInstance()->TriggerKey(DIK_P)) {
		whereNowSlot++;
		if (whereNowSlot >= 4) {
			whereNowSlot = 0;
		}
	}

	//選択されているものの切り替え
	state_.itemSlot[GetNowSlot()].isSelected = false;
	state_.itemSlot[whereNowSlot].isSelected = true;

	//選択されているものは大きく表示
	for (int i = 0; i < 4; i++) {
		if (state_.itemSlot[i].isSelected) {
			float selectedScale = 1.3f;
			state_.itemSlot[i].sprite[0].transform.scale = {selectedScale,selectedScale,selectedScale};
			state_.itemSlot[i].sprite[1].transform.scale = {selectedScale,selectedScale,selectedScale};
		}
		else {
			//非選択時は元に戻す
			state_.itemSlot[i].sprite[0].transform.scale = {1.0f,1.0f,1.0f};
			state_.itemSlot[i].sprite[1].transform.scale = {1.0f,1.0f,1.0f};
		}
	}

}

bool Player::StageChangeByDoor() {
	//ドア判定
	Segment playerEyeSegment;
	playerEyeSegment.start = model.transform.translate;
	playerEyeSegment.end = GetFrontVector(2.0f);
	//DoorとPlayerの視線の当たり判定を取得
	if(mapJson_->CheckCollisionWithEye(playerEyeSegment)) {
		canOpenDoor_ = true;
		return true;
	}
	else {
		canOpenDoor_ = false;
	}
	return false;
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
		int itemNum = 0;

	for (int i = 0; i < items_.size(); i++) {
		AABB itemAABB;
		itemAABB.CreateEntityAABB(items_[i]->model_.transform);

		Segment playerSight;
		playerSight.start = model.transform.translate;
		playerSight.end = GetFrontVector(2.0f);


		//視線との当たり判定
		if (itemAABB.CheckLineCollision(playerSight)) {
			//拾う処理
			//Fキーでアイテムを取得
			if (KeyInput::GetInstance()->TriggerKey(DIK_F)) {
				items_[i]->isTaken_ = true;
				items_[i]->whereSlot = CheckItemSlot();
				state_.itemSlot[CheckItemSlot()].isFilled = true;
			}
			canTakeItem_ = true;
			itemNum++;
		}
		if (itemNum <= 0) {
			canTakeItem_ = false;
		}

		//Item Update
		//所持されている状態 & 選択されているスロットにアイテムがある場合
		if (items_[i]->isTaken_) {
			items_[i]->model_.transform.translate = {};
			canDropItem_ = true;
			//選択されているスロットにアイテムがある場合
			if (state_.itemSlot[i].isFilled && GetNowSlot() == i) {
				if (KeyInput::GetInstance()->TriggerKey(DIK_G)) {
					items_[i]->DropItem(model.transform.translate);
					items_[i]->whereSlot = -1;
					items_[i]->isTaken_ = false;
					state_.itemSlot[i].isFilled = false;
				}
			}
			else {
				canDropItem_ = false;
			}
		}
	}
#ifdef _DEBUG

	ImGui::Begin("Item");

	ImGui::End();

#endif // DEBUG
}

int Player::CheckItemSlot() {
	//前から順に埋めていく
	if (!state_.itemSlot[0].isFilled) { //埋まってなかったら
		return 0;
	}
	else if (!state_.itemSlot[1].isFilled) { //埋まってなかったら
		return 1;
	}
	else if (!state_.itemSlot[2].isFilled) { //埋まってなかったら
		return 2;
	}
	else if (!state_.itemSlot[3].isFilled) { //埋まってなかったら
		return 3;
	}
	else {
		return -1;
	}
}

int Player::GetNowSlot() {
	if (state_.itemSlot[0].isSelected) {
		return 0;
	}
	else if (state_.itemSlot[1].isSelected) {
		return 1;
	}
	else if (state_.itemSlot[2].isSelected) {
		return 2;
	}
	else if (state_.itemSlot[3].isSelected) {
		return 3;
	}
	else {
		return -1;
	}
}


void Player::CheckItemBring(std::shared_ptr<mapItem> item) {
	//if (map_->GetItem()->isTaken_) {
	//	//所持しているとき手に持つ
	//	map_->GetItem()->model_.transform.translate = GetItemFrontVector();
	//	//PlayerのRotateと同期
	//	map_->GetItem()->model_.transform.rotate = model.transform.rotate;

	//	//
	//	if (KeyInput::GetInstance()->PushKey(DIK_G)) {//Drop処理
	//		map_->GetItem()->isTaken_ = false;
	//		//足元に落とす & リセット
	//		map_->GetItem()->model_.transform.translate = GetFrontVector(0.7f);
	//		map_->GetItem()->model_.transform.translate.y = 0.0f;
	//		map_->GetItem()->model_.transform.scale = { 1.0f,1.0f,1.0f };
	//		map_->GetItem()->model_.transform.rotate.x = {};
	//		map_->GetItem()->model_.transform.rotate.z = {};
	//	}
	//}
}

void Player::DeathUpdate() {
	if (!state_.isAlive) {
		deadFlame++;
		if (deadFlame >= 180) {
			deadFlame = 180;
		}
		deadModel.Update();

		deadCamera.translate = model.transform.translate;
		deadCamera.translate.y = 4.0f + float(3 * deadFlame)/180.0f;
		deadCamera.rotate.x = 1.27f;

		deadModel.transform.translate = model.transform.translate;
		deadModel.transform.rotate.y = 1.77f;

		deads_.materialData_->color.w = float(1+deadFlame)/180.0f;
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