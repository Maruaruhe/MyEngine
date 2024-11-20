#include "ShipStage.h"

ShipStage::~ShipStage(){}

void ShipStage::Initialize(int prevStage) {
	//Static Initialize
	player_.Initialize();
	camera2 = std::make_unique<Camera>();
	camera2->Initialize();

	light_.Initialize();

	//StageごとのInitialize
	if (isFirstLoad) {
		map_.mapData = map_.LoadMapData( "ship");
		isFirstLoad = false;
	}

	map_.Initialize(camera2.get());
	player_.SetMap(&map_);
	player_.model.SetCamera(camera2.get());
	player_.deadModel.SetCamera(camera2.get());
	player_.view.SetCamera(camera2.get());
	player_.model.transform.translate = { 4.0f,1.5f,-11.0f };

	stageChangeCount = 0;

	TextureManager::GetInstance()->LoadTexture("Resources/noise0.png");
	noise0.Initialize({ 1280,720 }, "Resources/noise0.png");

	 level = FileManager::GetInstance()->LoadJsonFile("Json/", "test3rd");
	 for (ObjectData& model : level.objects) {
		 model.model.SetCamera(camera2.get());
	 }
}


void ShipStage::Update() {
	light_.Update();

	camera2->Update();

	map_.Update();

	player_.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;

	noise0.Update();

	//mapModel.Update();
	for (ObjectData& model : level.objects) {
		model.model.Update();
	}
}


void ShipStage::Draw() {
	//map_.Draw();

	player_.Draw();

	//noise0.Draw();

	//mapModel.Draw();
	for (const ObjectData& model : level.objects) {
		model.model.Draw();
	}
}


void ShipStage::StageChange() {
	if(KeyInput::GetInstance()->TriggerKey(DIK_1)){
		stageNo = OUTSIDE;
	}

	if (KeyInput::GetInstance()->PushKey(DIK_E)) {
		stageChangeCount++;
		if (stageChangeCount >= 120) {
			stageNo = OUTSIDE;
		}
	}
	else {
		stageChangeCount = 0;
	}
}