#include "OutsideStage.h"

OutsideStage::~OutsideStage() {}

void OutsideStage::Initialize(int prevStage) {
	//StageごとのInitialize
	map_.Initialize(camera2.get(), "outside");

	player_.SetMap(&map_);
	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());
	if (prevStage == SHIP) {
		player_.model.transform.translate = { 0,0,0 };
	}
	else if (prevStage == INSIDE) {
		player_.model.transform.translate = { 10,0,-10 };
	}
}


void OutsideStage::Update() {
	light_.Update();

	camera2->Update();

	map_.Update();

	player_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;
}


void OutsideStage::Draw() {
	map_.Draw();

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