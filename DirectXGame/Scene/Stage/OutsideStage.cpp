#include "OutsideStage.h"

using namespace MyEngine;

OutsideStage::~OutsideStage() {}

void OutsideStage::FirstInitialize() {}

void OutsideStage::Initialize(int prevStage) {
	mapJson_.Initialize(camera2.get(), "OutsideStage");

	player_.SetMap(&mapJson_);

	light_.light->direction = { 0.95f, -0.33f, -0.02f };
	light_.light->intensity = 0.75f;


	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());

	//直前のStageによってPositionを設定
	if (prevStage == SHIP) {
		player_.model.transform.translate = { 0,1.5f,-8.0f };
		player_.model.transform.rotate = { 0.0f, 3.14f, 0.0f };
	}
	else if (prevStage == INSIDE) {
		player_.model.transform.translate = { 0,1.5f,-49 };
		player_.model.transform.rotate = { 0.0f, 0.0f, 0.0f };
	}

	stageChangeCount = 0;
}


void OutsideStage::Update() {
	light_.Update();

	camera2->Update();

	mapJson_.Update();

	player_.Update();


	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;
}


void OutsideStage::Draw() {
	player_.Draw();

	mapJson_.Draw();
}

void OutsideStage::SpriteDraw() {
	player_.SpriteDraw();
}

void OutsideStage::StageChange() {
	if (player_.StageChangeByDoor()) {
		if (KeyInput::GetInstance()->PushKey(DIK_R)) {
			stageChangeCount++;
			if (stageChangeCount >= 60) {
				if (player_.model.transform.translate.z >= -20.0f) {
					stageNo = SHIP;
				}
				else {
					stageNo = INSIDE;
				}
			}
		}
		else {
			stageChangeCount = 0;
		}
	}

	if (KeyInput::GetInstance()->TriggerKey(DIK_2)) {
		stageNo = SHIP;
	}
	if (KeyInput::GetInstance()->TriggerKey(DIK_3)) {
		stageNo = INSIDE;
	}
}