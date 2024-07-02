#include "TestScene.h"
#include "../Manager/ModelManager.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	camera2 = new Camera();
	camera2->Initialize();
	camera2->transform.translate.z = -10.0f;

	light.Initialize();

	ModelManager::GetInstance()->LoadModel("AnimatedCube");
	model.Initialize("AnimatedCube");
	model.SetCamera(camera2);
}

void TestScene::Update() {
//	GlobalVariables::GetInstance()->Update();

	light.Update();
	camera2->Update();

	model.Update();

	for (int i = 0; i < 3; ++i) {
		int a = i;
		int b = 0;
	}
}

void TestScene::Draw() {
	model.Draw();
}