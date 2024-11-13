#include "GameScene.h"
#include "../Manager/ModelManager.h"
#include "../Base/Input/Input.h"

void GameScene::Initialize() {
	stageArr[SHIP] = std::make_unique<ShipStage>();
	stageArr[OUTSIDE] = std::make_unique<OutsideStage>();
	stageArr[INSIDE] = std::make_unique<InsideStage>();
}

void GameScene::Update() {
	//Stageのチェック
	prevStageNo = currentStageNo;
	currentStageNo = stageArr[currentStageNo]->GetStageNo();

	//シーンが前フレームと異なったら初期化
	if (prevStageNo != currentStageNo) {
		stageArr[currentStageNo]->Initialize(prevStageNo);
	}

	stageArr[currentStageNo]->StageChange();
	stageArr[currentStageNo]->Update();
}

void GameScene::Draw() {
	stageArr[currentStageNo]->Draw();
}

void GameScene::SceneChange() {

}