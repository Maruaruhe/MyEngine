#include "TitleScene.h"
#include "../Manager/ModelManager.h"

void TitleScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Title/title.png");
	title.Initialize({0,0}, {1280,720}, "Resources/Title/title.png");
}

void TitleScene::Update() {
}

void TitleScene::Draw() {
	title.Draw();
}

void TitleScene::SceneChange() {
	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = INGAME;
	}
}