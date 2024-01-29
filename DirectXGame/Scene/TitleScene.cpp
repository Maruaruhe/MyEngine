#include "TitleScene.h"

void TitleScene::Initialize(){
	input = Input::GetInstance();
	input->Initialize();
}

void TitleScene::Update(){
	input->Update();

	if (input->PushKey(DIK_SPACE)) {
		sceneNo = INGAME;
	}
}

void TitleScene::Draw() {

}