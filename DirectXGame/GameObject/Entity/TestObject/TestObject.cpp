#include "TestObject.h"

void TestObject::Initialize() {
	ModelManager::GetInstance()->LoadModel("walk");
	anime.Initialize("walk");

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();
}

void TestObject::Update() {
	anime.Update();

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
}