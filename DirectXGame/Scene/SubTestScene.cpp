#include "SubTestScene.h"
#include "../Manager/ModelManager.h"

using namespace MyEngine;

void SubTestScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/ao.png");
	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

	a.Initialize({ 320.0f,180.0f }, "Resources/ao.png");
	b.Initialize({ 640.0f,360.0f }, "Resources/monsterBall.png");
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

void SubTestScene::SceneChange() {

}