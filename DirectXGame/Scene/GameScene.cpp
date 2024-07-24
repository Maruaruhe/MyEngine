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

	Transform trans;
	trans.translate = { 19.0f,0.0f,-18.0f };
	trans.scale = { 1.0f,1.0f,1.0f };
	trans.rotate = {};
	item.Initialize(trans, camera2.get());
}

void GameScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	map.Update();

	player.Update();
	camera2.get()->transform.translate = player.model.transform.translate;
	camera2.get()->transform.rotate = player.model.transform.rotate;

	item.Update();
}

void GameScene::Draw() {
	map.Draw();

	player.Draw();

	item.Draw();
}