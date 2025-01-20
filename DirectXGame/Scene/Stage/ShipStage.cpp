#include "ShipStage.h"

using namespace MyEngine;

ShipStage::~ShipStage(){}

void ShipStage::Initialize(int prevStage) {
	//Static Initialize
	player_.Initialize();
	camera2 = std::make_unique<Camera>();
	camera2->Initialize();

	light_.Initialize();
	light_.light->direction = { 0.275f,0.944f,0.181f };
	light_.light->intensity = 0.016f;

	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());
	player_.model.transform.translate = { 4.0f,1.5f,-11.0f };
	player_.model.transform.translate = { 0.0f,1.5f,0.0f };
	player_.model.transform.rotate = { 0.0f,0.0f,0.0f };

	stageChangeCount = 0;

	TextureManager::GetInstance()->LoadTexture("Resources/noise0.png");
	noise0.Initialize({ 1280,720 }, "Resources/noise0.png");

	mapJson_.Initialize(camera2.get(), "ShipStage");

	player_.SetMap(&mapJson_);

	trace_.Initialize({});
	trace_.SetPlayer(&player_);
	trace_.SetMap(&mapJson_);
	trace_.model_.SetCamera(camera2.get());

	stageChangeCount = 0;
}


void ShipStage::Update() {
	light_.Update();

	camera2->Update();

	player_.Update();

	trace_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;

	noise0.Update();

	mapJson_.Update();

	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		toClearScene = true;
	}
}


void ShipStage::Draw() {
	player_.Draw();

	trace_.Draw();

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