#include "OutsideStage.h"

using namespace MyEngine;

OutsideStage::~OutsideStage() {}

void OutsideStage::Initialize(int prevStage) {
	//StageごとのInitialize
	//if (isFirstLoad) {
	//	map_.mapData = map_.LoadMapData("outside");
	//	isFirstLoad = false;
	//}

	//map_.Initialize(camera2.get());

	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());

	//直前のStageによってPositionを設定
	if (prevStage == SHIP) {
		player_.model.transform.translate = { 0,1.5f,0 };
	}
	else if (prevStage == INSIDE) {
		player_.model.transform.translate = { 10,0,-10 };
	}
}


void OutsideStage::Update() {
	light_.Update();

	camera2->Update();

	player_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;
}


void OutsideStage::Draw() {
	player_.Draw();

}
void OutsideStage::StageChange() {
	if (KeyInput::GetInstance()->TriggerKey(DIK_2)) {
		stageNo = SHIP;
	}
	if (KeyInput::GetInstance()->TriggerKey(DIK_3)) {
		stageNo = INSIDE;
	}
}