#include "TestJsonObject.h"

void TestJsonObject::Initialize() {
	ModelManager::GetInstance()->LoadGLTF("sneakWalk");
	ModelManager::GetInstance()->LoadGLTF("simpleSkin");

	ModelManager::GetInstance()->LoadModel("ghostPori");
	model = std::make_unique<Model> ();
	model->Initialize("ghostPori");

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();
}

void TestJsonObject::Update() {
	model->Update();
}

void TestJsonObject::Draw() {
	model->Draw();
}