#include "TestScene.h"

void TestScene::Initialize() {
	input = Input::GetInstance();

	sphere.Initialize();

	sprite.Initialize({ 0,0 }, { 320,180 });

	model.Initialize("ghostPori");

	sphere.transform.translate = { -1.5f,0.0f,0.0f };
	model.transform.translate = { 1.5f,0.0f,0.0f };
	model.material->enableLighting = true;
}

void TestScene::Update() {
	GlobalVariables::GetInstance()->Update();

	sphere.Update();

	sprite.Update();

	model.Update();
}

void TestScene::Draw() {
	sphere.Draw();

	sprite.Draw();

	model.Draw();
}