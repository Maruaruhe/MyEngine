#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/title.png");

	title.Initialize({ 0,0 }, { 1280,720 }, "Resources/title.png");

}

void TestScene::Update() {

	title.Update();

	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = SUBTEST;
	}
}

void TestScene::Draw() {
	title.Draw();
}