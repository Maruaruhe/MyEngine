#include "TestScene.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	sphere.Initialize();
}

void TestScene::Update() {
	sphere.Update();
}

void TestScene::Draw() {
	sphere.Draw();
}