#include "ShipStage.h"

ShipStage::~ShipStage(){}

void ShipStage::Initialize(int prevStage) {
	//Static Initialize
	player_.Initialize();
	camera2 = std::make_unique<Camera>();
	camera2->Initialize();

	light_.Initialize();

	//StageごとのInitialize
	map_.Initialize(camera2.get(), "ship");

	player_.SetMap(&map_);
	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());
	player_.model.transform.translate = { 3.0f,1.5f,-4.0f };
}


void ShipStage::Update() {
	light_.Update();

	camera2->Update();

	map_.Update();

	player_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;
}


void ShipStage::Draw() {
	map_.Draw();

	player_.Draw();
}


void ShipStage::StageChange() {
	if(KeyInput::GetInstance()->TriggerKey(DIK_1)){
		stageNo = OUTSIDE;
	}
}