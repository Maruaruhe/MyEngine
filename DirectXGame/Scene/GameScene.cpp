#include "GameScene.h"
#include "../Manager/ModelManager.h"
#include "../Base/Input/Input.h"

void GameScene::Initialize() {
	TextureManager::GetInstance()->LoadTexture("Resources/Title/title.dds");
	s.Initialize({ 1280,720 }, "Resources/Title/title.dds");
	s.transform.translate = { 640,360 ,0 };
	s.anchorPoint = { 0.5f,0.5f };

	camera.Initialize();
	camera.transform.translate.z = -10.0f;
	ModelManager::GetInstance()->LoadModel("player");
	model.Initialize("player");
	model.SetCamera(&camera);
}

void GameScene::Update() {
	s.Update();

	camera.Update();
	model.Update();
}

void GameScene::Draw() {
	//s.Draw();

	model.Draw();
}

void GameScene::SceneChange() {

}