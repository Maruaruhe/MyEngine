#include "GameScene.h"
#include "../Manager/ModelManager.h"
#include "../Base/Input/Input.h"

void GameScene::Initialize() {
	TextureManager::GetInstance()->LoadTexture("Resources/Title/title.dds");
	s.Initialize({ 1280,720 }, "Resources/Title/title.dds");
	s.transform.translate = { 640,360 ,0 };
	s.anchorPoint = { 0.5f,0.5f };
}

void GameScene::Update() {
	s.Update();
}

void GameScene::Draw() {
	s.Draw();
}

void GameScene::SceneChange() {

}