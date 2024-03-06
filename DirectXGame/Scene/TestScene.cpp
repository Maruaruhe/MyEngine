#include "TestScene.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	camera = new Camera();
	camera->Initialize();
	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	//sphere.SetCamera(camera2);
	//sphere.Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/ao.png");
	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

	sprite.Initialize({ 0,0 }, { 320,180 } , "Resources/ao.png");
	//a.Initialize({ 320,180 }, { 640,360 }, "Resources/monsterBall.png");

	model.SetCamera(camera2);
	model.Initialize("ghostPori");
	
	//model2.SetCamera(camera2);
	//model2.Initialize("axis");

	//sphere.transform.translate = { -1.5f,0.0f,0.0f };

	model.transform.translate = { 1.5f,0.0f,0.0f };
	model.material->enableLighting = true;

	//model2.transform.translate = { -1.5f,0.0f,0.0f };
	//model2.material->enableLighting = true;

	light.Initialize();
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();
//	model.SetCamera(camera2);

	light.Update();
	camera->Update();
	camera2->Update();

	//sphere.Update();

	sprite.Update();
	//a.Update();

	model.Update();
//	model2.Update();
}

void TestScene::Draw() {
	//sphere.Draw();

	sprite.Draw();
	//a.Draw();

	model.Draw();
	//model2.Draw();
}