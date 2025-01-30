#include "TitleScene.h"
#include "../Manager/ModelManager.h"

using namespace MyEngine;

TitleScene::~TitleScene() {

}

void TitleScene::Initialize() {
	input = KeyInput::GetInstance();

	phase_ = first;

	sceneMove = false;

	TextureManager::GetInstance()->LoadTexture("Resources/Title/background.png");
	back.Initialize({ 1280,720 }, "Resources/Title/background.png");
	back.transform.translate = { 640,360 ,0 };
	back.anchorPoint = { 0.5f,0.5f };

	TextureManager::GetInstance()->LoadTexture("Resources/Title/titletmp.png");
	title.Initialize({1280,720}, "Resources/Title/titletmp.png");
	title.transform.translate = { 640,360 ,0};
	title.anchorPoint = { 0.5f,0.5f };

	TextureManager::GetInstance()->LoadTexture("Resources/Title/explain.png");
	explain.Initialize({1280,720}, "Resources/Title/explain.png");
	explain.transform.translate = { 640,360 ,0};
	explain.transform.scale = { 0.0,0.0 ,0};
	explain.anchorPoint = { 0.5f,0.5f };
}

void TitleScene::Update() {
	back.Update();
	title.Update();
	explain.Update();

	if (input->TriggerKey(DIK_SPACE)) {
		if (title.transform.scale.x > 0.0f) {
			startTitleChanging = true;
		}
	}
	if (startTitleChanging) {
		title.transform.scale.x -= 0.1f;
		title.transform.scale.y -= 0.1f;
		if (title.transform.scale.x <= 0.0f) {
			startTitleChanging = false; 
			startExplainChanging = true;
		}
	}
	if (startExplainChanging) {
		explain.transform.scale.x += 0.1f;
		explain.transform.scale.y += 0.1f;
		if (explain.transform.scale.x >= 1.0f) {
			startExplainChanging = false;
			sceneMove = true;
		}
	}
}

void TitleScene::Draw() {
	back.Draw();
	title.Draw();
	explain.Draw();
}

void TitleScene::SceneChange() {
	if (sceneMove && input->TriggerKey(DIK_SPACE)) {
		sceneNo = INGAME;
	}
}