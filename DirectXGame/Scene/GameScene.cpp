#include "GameScene.h"
#include "../Manager/ModelManager.h"

void GameScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = std::make_unique<Camera>();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	wall.Initialize({0.0f,-1.0f,0.0f}, {5.0f,5.0f,1.0f});
	wall.model.SetCamera(camera2.get());	
	
	wall2.Initialize({-1.0f,-1.0f,0.0f}, {1.0f,5.0f,1.0f});
	wall2.model.SetCamera(camera2.get());
}

void GameScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	wall.Update();
	wall2.Update();
}

void GameScene::Draw() {
	wall.Draw();
	//wall2.Draw();
}