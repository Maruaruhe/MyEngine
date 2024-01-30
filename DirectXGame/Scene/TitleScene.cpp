#include "TitleScene.h"

void TitleScene::Initialize(){
	input = Input::GetInstance();
	black.Initialize({-1280,0},{0,720});
	title.Initialize({ 0,0 }, { 1280,720 });

	scroll = 0;
	isScroll = false;
}

void TitleScene::Update(){
	if (input->TriggerKey(DIK_SPACE)) {
		isScroll = true;
	}
	if (isScroll) {
		scroll += scrollSpeed;
	}
	if (scroll >= 1280) {
		sceneNo = INGAME;
	}
	Vector2 scrollPosition = { float(scroll),0 };
	black.SetPosition(scrollPosition);
	title.Update();
	black.Update();
}

void TitleScene::Draw() {
	title.Draw();
	black.Draw();
}