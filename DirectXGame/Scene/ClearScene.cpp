#include "ClearScene.h"
#include "../Manager/ModelManager.h"

void ClearScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Title/clear.png");
	title.Initialize({ 0,0 }, { 1280,720 }, "Resources/Title/clear.png");
}

void ClearScene::Update() {
	title.Update();
}

void ClearScene::Draw() {
	title.Draw();
}

void ClearScene::SceneChange() {
	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}
}