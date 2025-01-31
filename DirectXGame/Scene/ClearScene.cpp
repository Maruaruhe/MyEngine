#include "ClearScene.h"
#include "../Manager/ModelManager.h"

using namespace MyEngine;

void ClearScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Title/clear.png");
	title.Initialize({ 1280,720 }, "Resources/Title/clear.png");
	title.anchorPoint = {};

	TextureManager::GetInstance()->LoadTexture("Resources/Title/black.png");
	black.Initialize({ 1280,720 }, "Resources/Title/black.png");
}

void ClearScene::Update() {
	title.Update();
	black.Update();
	black.materialData_->color.w -= 0.005f;
}

void ClearScene::Draw() {
	title.Draw();
	//black.Draw();
}

void ClearScene::SceneChange() {
	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}
}