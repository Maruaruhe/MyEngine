#include "ShipStage.h"

using namespace MyEngine;

ShipStage::~ShipStage(){}

void ShipStage::FirstInitialize(Player* player) {
	mapJson_.Initialize(camera2.get(), "ShipStage", player);
}

void ShipStage::Initialize(int prevStage) {
	//Static Initialize
	light_.light->direction = { 0.95f, -0.33f, -0.02f };
	light_.light->intensity = 0.75f;

	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());
	player_.model.transform.translate = { 4.0f,1.5f,-11.0f };
	player_.model.transform.translate = { 0.0f,1.5f,0.0f };
	player_.model.transform.rotate = { 0.0f,3.14f,0.0f };

	stageChangeCount = 0;

	player_.SetMap(&mapJson_);

	stageChangeCount = 0;
}


void ShipStage::Update() {
	light_.Update();

	camera2->Update();

	player_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;

	mapJson_.Update();

	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		toClearScene = true;
	}
}


void ShipStage::Draw() {
	player_.Draw();

	mapJson_.Draw();
}

void ShipStage::SpriteDraw(){
	player_.SpriteDraw();
}

void ShipStage::StageChange() {
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

	if(KeyInput::GetInstance()->TriggerKey(DIK_1)){
		stageNo = OUTSIDE;
	}

}