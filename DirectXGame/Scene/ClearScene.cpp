#include "ClearScene.h"
void ClearScene::Initialize() {
	input = Input::GetInstance();
}

void ClearScene::Update() {
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}
}

void ClearScene::Draw() {

}