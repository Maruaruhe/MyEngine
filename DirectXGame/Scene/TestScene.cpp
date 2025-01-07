#include "TestScene.h"
#include "../Manager/ModelManager.h"
#include "../Manager/FileManager.h"

using namespace MyEngine;

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

	s.Initialize();
	s.camera = camera2.get();
	test.model.SetCamera(camera2.get());
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	test.Update();
	s.Update();
}

void TestScene::Draw() {
	test.Draw();
	//s.Draw();
}

void TestScene::SceneChange() {

}