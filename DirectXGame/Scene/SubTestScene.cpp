#include "SubTestScene.h"
#include "../Manager/ModelManager.h"

void SubTestScene::Initialize() {
	input = Input::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/ingame.png");

	ingame.Initialize({ 0,0 }, { 1280,720 }, "Resources/ingame.png");
}

void SubTestScene::Update() {
	ingame.Update();

	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = 1;
	}
}

void SubTestScene::Draw() {
	ingame.Draw();
}