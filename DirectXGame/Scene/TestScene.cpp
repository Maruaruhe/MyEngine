#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	ModelManager::GetInstance()->LoadModel("walk");
	//ModelManager::GetInstance()->LoadModel("GLTFPlane");
	ModelManager::GetInstance()->LoadModel("plane");

	model.Initialize("walk");
	model.SetCamera(camera2);

	//a.Initialize("plane");
	//a.SetCamera(camera2);
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	model.Update();
	//a.Update();
}

void TestScene::Draw() {
	model.Draw();
	//a.Draw();
}