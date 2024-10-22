#include "GameScene.h"
#include "../Manager/ModelManager.h"
#include "../Base/Input/Input.h"

void GameScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = std::make_unique<Camera>();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	map.Initialize(camera2.get());

	player.Initialize();
	player.SetMap(&map);
	player.model.SetCamera(camera2.get());
	player.view.SetCamera(camera2.get());

	//trace.Initialize({ {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, { -10.0f, 0.0f, 1.0f } });

	sprite.Initialize({ 360,180 }, "Resources/explain.png");
	s.Initialize({ 100,100 }, "Resources/Title/titleHeart.png");
	s.transform.translate = { 640,360, 0 };
	s.anchorPoint = { 0.5,0.5 };

	isS = true;
	sFlame = 0;
}

void GameScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();

	camera2->Update();

	map.Update();

	player.Update();

	//trace.Update();

	camera2.get()->transform.translate = player.model.transform.translate;
	camera2.get()->transform.rotate = player.model.transform.rotate;

	sprite.Update();

	if (isS) {
		sFlame++;
		s.transform.scale *= 1.2f;
		if (sFlame >= 45) {
			isS = false;
		}
	}
}

void GameScene::Draw() {
	map.Draw();

	player.Draw();

	//trace.Draw();

	sprite.Draw();
	if (isS) {
		s.Draw();
	}
}

void GameScene::SceneChange() {
	if (map.GetItem()->model.transform.translate.x >= 17.00f && map.GetItem()->model.transform.translate.x <= 19.00f) {
		if (map.GetItem()->model.transform.translate.z >= -12.5f && map.GetItem()->model.transform.translate.z <= -10.5f) {
			if (map.GetItem()->isTaken == false) {
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
}