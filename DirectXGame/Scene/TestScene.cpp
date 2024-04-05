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

	p.Initialize("plane");
	modela.Initialize("axis");
	model.Initialize("ghostPori");

	p.SetCamera(camera2);
	modela.SetCamera(camera2);
	model.SetCamera(camera2);

	p.material->enableLighting = false;

	modela.transform.translate = { 1.5f,0.0f,0.0f };
	modela.material->enableLighting = true;
	model.transform.translate = { 1.5f,0.0f,0.0f };
	model.material->enableLighting = true;

	TextureManager::GetInstance()->LoadTexture("Resources/ao.png");
	sprite.Initialize({ 0,0 }, { 320,180 }, "Resources/ao.png");

//	light.Initialize();
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

//	light.Update();
	camera2->Update();

	p.Update();
	modela.Update();
	model.Update();

	sprite.Update();

	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = SUBTEST;
	}

}

void TestScene::Draw() {
	modela.Draw();
	model.Draw();

	p.Draw();

	sprite.Draw();
}