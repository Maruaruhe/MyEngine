#include "TestObject.h"

void TestObject::Initialize() {
	ModelManager::GetInstance()->LoadGLTF("sneakWalk");
	anime.Initialize("sneakWalk");

	ModelManager::GetInstance()->LoadModel("ghostPori");
	model.Initialize("ghostPori");

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();
}

void TestObject::Update() {
	anime.Update();
	anime.transform.rotate.y += 0.05f;

	model.Update();
	model.transform.rotate.y += 0.05f;

	//if (kInput->PushKey(DIK_UP)) {
	//	anime.transform.translate.y += 0.1f;
	//}

	float spd = 0.1f;
	// パッドの処理
	if (pInput->GetLStick().x <= -0.3f)
	{
		anime.transform.translate.x -= spd;
	}
	if (pInput->GetLStick().x >= 0.3f)
	{
		anime.transform.translate.x += spd;
	}
	if (pInput->GetLStick().y <= -0.3f)
	{
		anime.transform.translate.z -= spd;
	}
	if (pInput->GetLStick().y >= 0.3f)
	{
		anime.transform.translate.z += spd;
	}
}

void TestObject::Draw() {
	anime.Draw();

	model.Draw();
}