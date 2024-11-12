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
	//if (insideMap.GetItem()->model.transform.translate.x >= 17.00f && insideMap.GetItem()->model.transform.translate.x <= 19.00f) {
	//	if (insideMap.GetItem()->model.transform.translate.z >= -12.5f && insideMap.GetItem()->model.transform.translate.z <= -10.5f) {
	//		if (insideMap.GetItem()->isTaken == false) {
	//			clearCount++;
	//			if (clearCount >= 60) {
	//				sceneNo = CLEAR;
	//			}
	//		}
	//	}
	//}
	//else {
	//	clearCount = 0;
	//}

	//if (player.deadFlame >= 180) {
	//		sceneNo = TITLE;
	//}
}