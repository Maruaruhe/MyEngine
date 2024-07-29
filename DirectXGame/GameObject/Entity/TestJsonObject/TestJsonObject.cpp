#include "TestJsonObject.h"
#include "../../../Manager/FileManager.h"

void TestJsonObject::Initialize() {
	ModelManager::GetInstance()->LoadModel("Box");

	model = std::make_unique<Model> ();
	model->Initialize("Box");
	model->transform = FileManager::GetInstance()->GetObjectTransform("Stick");

	model2 = std::make_unique<Model>();
	model2->Initialize("Box");
	model2->transform = FileManager::GetInstance()->GetObjectTransform("Ita");

	model3 = std::make_unique<Model>();
	model3->Initialize("Box");
	model3->transform = FileManager::GetInstance()->GetObjectTransform("Yuka");

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();
}

void TestJsonObject::Update() {
	model->Update();
	model2->Update();
	model3->Update();
}

void TestJsonObject::Draw() {
	model->Draw();
	model2->Draw();
	model3->Draw();
}