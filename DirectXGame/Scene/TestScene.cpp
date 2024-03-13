#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	sphere.SetCamera(camera2);
	sphere.Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/ao.png");
	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

	ModelManager::GetInstance()->LoadModel("ghostPori");
	//ModelManager::GetInstance()->LoadModel("axis");

	model.Initialize("ghostPori");
	//modela.Initialize("axis");
	model.SetCamera(camera2);

	sphere.transform.translate = { -1.5f,0.0f,0.0f };
	model.transform.translate = { 1.5f,0.0f,0.0f };
	model.material->enableLighting = true;


	light.Initialize();
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	sphere.Update();

	model.Update();
}

void TestScene::Draw() {
	sphere.Draw();

	model.Draw();
}