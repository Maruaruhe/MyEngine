#include "TitleScene.h"

void TitleScene::Initialize(){
	input = Input::GetInstance();
	sprite.Initialize({-640,-360,0},{640,360,0});
	camera.Initialize();
}

void TitleScene::Update(){
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = INGAME;
	}
	Vector4 a = { 1,1,1,1 };
	sprite.Update(a,camera.GetTransform());
}

void TitleScene::Draw() {
	sprite.Draw();
}