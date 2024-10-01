#include "ClearScene.h"
#include "../Manager/ModelManager.h"

void ClearScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Title/clear.png");
	title.Initialize({ 0,0 }, { 1280,720 }, "Resources/Title/clear.png");
}

void ClearScene::Update() {
	title.Update();

	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}
}

void ClearScene::Draw() {
	title.Draw();
}