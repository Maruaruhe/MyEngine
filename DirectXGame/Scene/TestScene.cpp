#include "TestScene.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	sphere.Initialize();

	model.Initialize("ghostPori");

	sphere.transform.translate = { -1.5f,0.0f,0.0f };
	model.transform.translate = { 1.5f,0.0f,0.0f };
}

void TestScene::Update() {
	sphere.Update();

	model.Update();
}

void TestScene::Draw() {
	sphere.Draw();

	model.Draw();
}