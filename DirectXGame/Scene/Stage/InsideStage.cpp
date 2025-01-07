#include "InsideStage.h"
using namespace MyEngine;

InsideStage::~InsideStage() {}

void InsideStage::Initialize(int prevStage) {
	//StageごとのInitialize
	//if (isFirstLoad) {
	//	map_.mapData = map_.LoadMapData("50x50");
	//	isFirstLoad = false;
	//}

	//map_.Initialize(camera2.get());

	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());
	player_.model.transform.translate = { 3.0f,1.5f,-4.0f };
}
void InsideStage::Update() {
	light_.Update();

	camera2->Update();

	player_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;
}
void InsideStage::Draw() {
	player_.Draw();
}
void InsideStage::StageChange() {
	if (KeyInput::GetInstance()->TriggerKey(DIK_4)) {
		stageNo = OUTSIDE;
	}
}