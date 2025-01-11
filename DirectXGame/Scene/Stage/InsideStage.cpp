#include "InsideStage.h"
using namespace MyEngine;

InsideStage::~InsideStage() {}

void InsideStage::Initialize(int prevStage) {
	mapJson_.Initialize(camera2.get(), "Inside");

	player_.SetMap(&mapJson_);

	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());
	player_.model.transform.translate = { 0.0f,1.5f,2.0f };

	stageChangeCount = 0;
}


void InsideStage::Update() {
	light_.Update();

	camera2->Update();

	player_.Update();

	mapJson_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;
}


void InsideStage::Draw() {
	player_.Draw();

	mapJson_.Draw();
}


void InsideStage::StageChange() {
	if (player_.StageChangeByDoor()) {
		if (KeyInput::GetInstance()->PushKey(DIK_R)) {
			stageChangeCount++;
			if (stageChangeCount >= 60) {
				stageNo = OUTSIDE;
			}
		}
		else {
			stageChangeCount = 0;
		}
	}

	if (KeyInput::GetInstance()->TriggerKey(DIK_4)) {
		stageNo = OUTSIDE;
	}
}