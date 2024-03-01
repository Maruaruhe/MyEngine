#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	sphere.Initialize();

	sprite.Initialize({ 0,0 }, { 320,180 } , "Resources/ao.png");
	a.Initialize({ 320,180 }, { 640,360 }, "Resources/monsterBall.png");

	ModelManager::GetInstance()->LoadModel("ghostPori");
	ModelManager::GetInstance()->LoadModel("axis");

	model.Initialize("ghostPori");
	modela.Initialize("axis");

	//ModelManager::GetInstance()->LoadModel("plane.obj");

	sphere.transform.translate = { -1.5f,0.0f,0.0f };
	//model.transform.translate = { 1.5f,0.0f,0.0f };
	//model.material->enableLighting = true;

	light.Initialize();
	camera.Initialize();
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera.Update();

	sphere.Update();

	//sprite.Update();
	a.Update();

	model.Update();
	modela.Update();
}

void TestScene::Draw() {
	sphere.Draw();

	//sprite.Draw();
	a.Draw();

	model.Draw();
	modela.Draw();
}