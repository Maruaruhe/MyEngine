#include "TitleScene.h"
#include "../Manager/ModelManager.h"

void TitleScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Title/title.png");
	title.Initialize({0,0}, {1280,720}, "Resources/Title/title.png");
}

void TitleScene::Update() {
	title.Update();

	if (KeyInput::GetInstance()->PushKey(DIK_SPACE)) {
  		sceneNo = INGAME;
	}
}

void TitleScene::Draw() {
	title.Draw();
}