#include "GameScene.h"
#include "../Manager/ModelManager.h"

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

	//trace.Initialize({ {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, { -10.0f, 0.0f, 1.0f } }, camera2.get(), &map, &player);

	sprite.Initialize({}, { 360,180 }, "Resources/explain.png");
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



	if (map.GetItem()->model.transform.translate.x >= 1.25f && map.GetItem()->model.transform.translate.x <= 2.75f ) {
		if (map.GetItem()->model.transform.translate.x >= -3.5f) {
			if (map.GetItem()->isTaken == false) {
				clearCount++;
				if (clearCount >= 60) {
					sceneNo = TITLE;
				}
			}
		}
	}
	else {
		clearCount = 0;
	}

	sprite.Update();
}

void GameScene::Draw() {
	map.Draw();

	player.Draw();

	//trace.Draw();

	sprite.Draw();
}