#include "GameScene.h"
#include "../Manager/ModelManager.h"
#include "../Base/Input/Input.h"

void GameScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = std::make_unique<Camera>();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	outsideMap.Initialize(camera2.get(), "50x50");
	insideMap.Initialize(camera2.get(), "50x50");

	player.Initialize();
	player.SetMap(&insideMap);
	player.model.SetCamera(camera2.get());
	player.deadModel.SetCamera(camera2.get());
	player.view.SetCamera(camera2.get());

	//trace.Initialize({ {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, { -10.0f, 0.0f, 1.0f } });

	sprite.Initialize({ 360,180 }, "Resources/explain.png");
	s.Initialize({ 100,100 }, "Resources/Title/titleHeart.png");

	s.transform.translate = { 640,360, 0 };
	s.anchorPoint = { 0.5,0.5 };

	isS = true;
	sFlame = 0;

	stage = SHIP;
}

void GameScene::Update() {
	preStage = stage;

	if (stage == SHIP) {
		if(input->TriggerKey(DIK_SPACE)){
			stage = INSIDE;
		}
	}

	if (stage == OUTSIDE) {

	}

	if (stage == INSIDE) {
		GlobalVariables::GetInstance()->Update();

		light.Update();

		camera2->Update();

		insideMap.Update();

		player.Update();

		//trace.Update();

		camera2.get()->transform.translate = player.tForCamera.translate;
		camera2.get()->transform.translate.y += 0.5f;
		camera2.get()->transform.rotate = player.tForCamera.rotate;

		sprite.Update();
	}
}

void GameScene::Draw() {
	if (stage == SHIP) {

	}

	if (stage == OUTSIDE) {

	}

	if (stage == INSIDE) {
		insideMap.Draw();

		player.Draw();
	}

	//trace.Draw();

	//sprite.Draw();
}

void GameScene::SceneChange() {
	if (insideMap.GetItem()->model.transform.translate.x >= 17.00f && insideMap.GetItem()->model.transform.translate.x <= 19.00f) {
		if (insideMap.GetItem()->model.transform.translate.z >= -12.5f && insideMap.GetItem()->model.transform.translate.z <= -10.5f) {
			if (insideMap.GetItem()->isTaken == false) {
				clearCount++;
				if (clearCount >= 60) {
					sceneNo = CLEAR;
				}
			}
		}
	}
	else {
		clearCount = 0;
	}

	if (player.deadFlame >= 180) {
			sceneNo = TITLE;
	}
}