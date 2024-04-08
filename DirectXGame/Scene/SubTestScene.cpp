#include "SubTestScene.h"
#include "../Manager/ModelManager.h"

void SubTestScene::Initialize() {
	input = Input::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/ao.png");
	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

	a.Initialize({ 0.0f,0.0f }, { 320.0f,180.0f }, "Resources/ao.png");
	b.Initialize({ 320.0f,180.0f }, { 640.0f,360.0f }, "Resources/monsterBall.png");
}

void SubTestScene::Update() {

	a.Update();
	b.Update();

	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = TEST;
	}
}

void SubTestScene::Draw() {

	a.Draw();
	b.Draw();

}