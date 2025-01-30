#include "GameScene.h"
#include "../Manager/ModelManager.h"
#include "../Base/Input/Input.h"

void GameScene::Initialize() {
	stageArr[SHIP] = std::make_unique<ShipStage>();
	stageArr[OUTSIDE] = std::make_unique<OutsideStage>();
	stageArr[INSIDE] = std::make_unique<InsideStage>();

	stageArr[SHIP]->GameInitialize();

	stageArr[SHIP]->FirstInitialize({});
	stageArr[OUTSIDE]->FirstInitialize({});
	stageArr[INSIDE]->FirstInitialize({});
}

void GameScene::Update() {
	//Stageのチェック
	prevStageNo = currentStageNo;
	currentStageNo = stageArr[currentStageNo]->GetStageNo();

	//シーンが前フレームと異なったら初期化
	if (prevStageNo != currentStageNo) {

		stageArr[currentStageNo]->Initialize(prevStageNo);
	}

#ifdef _DEBUG
	ImGui::Begin("Stage");
	ImGui::Text("Stage : %d", currentStageNo);
	ImGui::End();
#endif // _DEBUG


	stageArr[currentStageNo]->StageChange();
	stageArr[currentStageNo]->Update();
	stageArr[currentStageNo]->ItemUpdate(currentStageNo);

#ifdef _DEBUG
	stageArr[currentStageNo]->TimeLapse();
#endif // _DEBUG
}

void GameScene::Draw() {
	stageArr[currentStageNo]->Draw();
	stageArr[currentStageNo]->ItemDraw(currentStageNo);
	stageArr[currentStageNo]->SpriteDraw();
}

void GameScene::SceneChange() {
	if (stageArr[currentStageNo]->GetToClear()) {
		//sceneNo = CLEAR;
	}
}