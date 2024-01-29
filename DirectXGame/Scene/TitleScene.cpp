#include "TitleScene.h"

void TitleScene::Initialize(){

}

void TitleScene::Update(){
	if (input->PushKey(DIK_SPACE)) {
		sceneNo = INGAME;
	}
}

void TitleScene::Draw() {

}

void TitleScene::BeginFrame(){}
void TitleScene::EndFrame(){}
void TitleScene::Final(){}