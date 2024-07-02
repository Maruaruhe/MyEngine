#include "TestObject.h"

void TestObject::Initialize() {
	ModelManager::GetInstance()->LoadGLTF("sneakWalk");
	ModelManager::GetInstance()->LoadGLTF("simpleSkin");
	anime.Initialize("simpleSkin");
	anime2.Initialize("sneakWalk");
	anime3.Initialize("sneakWalk");

	anime.transform.rotate.y += 3.14f;
	anime2.transform.translate.x = -1.5f;
	anime3.transform.translate.x = -3.0f;
	//anime2.isPlay = false;

	anime3.animationSpeed = 2.0f;

	ModelManager::GetInstance()->LoadModel("ghostPori");
	model.Initialize("ghostPori");

	kInput = KeyInput::GetInstance();
	pInput = GamePadInput::GetInstance();
}

void TestObject::Update() {
	anime.Update();

	anime2.Update();
	anime3.Update();

	model.Update();
	model.transform.rotate.y += 0.05f;

	//float spd = 0.1f;
	//// パッドの処理
	//if (pInput->GetLStick().x <= -0.3f)
	//{
	//	anime.transform.translate.x -= spd;
	//}
	//if (pInput->GetLStick().x >= 0.3f)
	//{
	//	anime.transform.translate.x += spd;
	//}
	//if (pInput->GetLStick().y <= -0.3f)
	//{
	//	anime.transform.translate.z -= spd;
	//}
	//if (pInput->GetLStick().y >= 0.3f)
	//{
	//	anime.transform.translate.z += spd;
	//}
}

void TestObject::Draw() {
	anime.Draw();
	anime2.Draw();
	anime3.Draw();

	model.Draw();
}