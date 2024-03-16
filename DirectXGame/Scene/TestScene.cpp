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
	ModelManager::GetInstance()->LoadModel("axis");
	ModelManager::GetInstance()->LoadModel("plane");

	p.Initialize("plane");
	modela.Initialize("axis");

	p.SetCamera(camera2);
	modela.SetCamera(camera2);

	sphere.transform.translate = { -1.5f,0.0f,0.0f };
	p.transform.translate = { 1.5f,0.0f,0.0f };
	p.material->enableLighting = false;
	modela.transform.translate = { 1.5f,0.0f,0.0f };
	modela.material->enableLighting = true;

	sprite.Initialize({ 0,0 }, { 360,180 }, "Resources/ao.png");


	light.Initialize();
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	sphere.Update();

	p.Update();
	modela.Update();

	sprite.Update();
}

void TestScene::Draw() {
	sphere.Draw();

	p.Draw();
	modela.Draw();

	sprite.Draw();
}