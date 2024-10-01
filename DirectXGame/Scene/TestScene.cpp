#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	test.Initialize();
	test.anime.SetCamera(camera2);
	test.anime2.SetCamera(camera2);
	test.anime3.SetCamera(camera2);

	test.model.SetCamera(camera2);
	test.model2.SetCamera(camera2);

	s.Initialize();
	s.camera = camera2;

	sprite.Initialize({}, { 360,180 }, "Resources/ao.png");
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	test.Update();

	s.Update();

	sprite.Update();
}

void TestScene::Draw() {
	test.Draw();

	s.Draw();

	sprite.Draw();
}