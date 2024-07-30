#include "TestScene.h"
#include "../Manager/ModelManager.h"
#include "../Manager/FileManager.h"

void TestScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = std::make_unique<Camera>();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	test.Initialize();
	test.anime.SetCamera(camera2.get());
	test.anime2.SetCamera(camera2.get());
	test.anime3.SetCamera(camera2.get());
	FileManager::GetInstance()->LoadJsonFile("Json/", "testJ");
	testJson.Initialize();
	testJson.model->SetCamera(camera2);
	testJson.model2->SetCamera(camera2);
	testJson.model3->SetCamera(camera2);

	s.Initialize();
	s.camera = camera2;
	test.model.SetCamera(camera2.get());
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	testJson.Update();

	s.Update();
}

void TestScene::Draw() {
	testJson.Draw();

	//s.Draw();
}