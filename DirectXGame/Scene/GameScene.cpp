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
	player.model.SetCamera(camera2.get());
}

void GameScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	map.Update();

	player.Update();
	camera2.get()->transform.translate = player.model.transform.translate;
	camera2.get()->transform.rotate = player.model.transform.rotate;
}

void GameScene::Draw() {
	map.Draw();

	player.Draw();
}