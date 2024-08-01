#include "TestScene.h"
#include "../Manager/ModelManager.h"
#include "../Manager/FileManager.h"

void TestScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	FileManager::GetInstance()->LoadJsonFile("Json/", "loadMap");
	FileManager::GetInstance()->CreateModels(camera2);
	//testJson.Initialize();
	//testJson.model->SetCamera(camera2);
	//testJson.model2->SetCamera(camera2);
	//testJson.model3->SetCamera(camera2);

	s.Initialize();
	s.camera = camera2;
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	FileManager::GetInstance()->Update();
	//testJson.Update();

	//s.Update();
}

void TestScene::Draw() {
	FileManager::GetInstance()->Draw();

	//testJson.Draw();

	//s.Draw();
}