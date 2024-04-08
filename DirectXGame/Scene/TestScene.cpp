#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	ModelManager::GetInstance()->LoadModel("ghostPori");
	ModelManager::GetInstance()->LoadModel("axis");
	ModelManager::GetInstance()->LoadModel("plane");

	p.Initialize("Resources/ao.png");
	p.SetCamera(camera2);
	p.material->enableLighting = false;

	modela.Initialize("axis");

	modela.SetCamera(camera2);
	
	modela.transform.translate = { 1.5f,0.0f,0.0f };
	modela.material->enableLighting = true;

	light.Initialize();
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	p.Update();


	modela.Update();

	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = SUBTEST;
	}

}

void TestScene::Draw() {
	modela.Draw();

	p.Draw();
}