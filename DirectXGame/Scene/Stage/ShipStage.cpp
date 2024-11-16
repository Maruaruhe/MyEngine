#include "ShipStage.h"
#include "../../Manager/FileManager.h"

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
	player_.model.transform.translate = { 3.0f,1.5f,-4.0f };
	player_.model.transform.translate = { 0.0f,0.0f,0.0f };

	CreateModels();
}


void ShipStage::Update() {
	light_.Update();

	camera2->Update();

	map_.Update();

	player_.Update();

	mapModel.Update();

	camera2.get()->transform.translate = player_.tForCamera.translate;
	camera2.get()->transform.translate.y += 0.5f;
	camera2.get()->transform.rotate = player_.tForCamera.rotate;
}


void ShipStage::Draw() {
	//map_.Draw();

	player_.Draw();

	mapModel.Draw();
}


void ShipStage::StageChange() {
	if(KeyInput::GetInstance()->TriggerKey(DIK_1)){
		stageNo = OUTSIDE;
	}
}

void ShipStage::CreateModels() {
	FileManager::GetInstance()->LoadJsonFile("Json/", "mapWall");
	ModelManager::GetInstance()->LoadModel("mapWall");
	mapModel.Initialize("mapWall");
	mapModel.SetCamera(camera2.get());
	mapModel.transform = FileManager::GetInstance()->GetObjectTransform("mapWall");
}