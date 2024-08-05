#include "TitleScene.h"
#include "../Manager/ModelManager.h"

void TitleScene::Initialize() {
	input = KeyInput::GetInstance();

	camera2 = std::make_unique<Camera>();
	camera2->Initialize();
	camera2->transform.translate.z = -15.0f;

	ModelManager::GetInstance()->LoadModel("a");
	model.Initialize("a");
	model.transform.rotate.y = 1.57f;
	model.transform.translate.y -= 1.5f;

	TextureManager::GetInstance()->LoadTexture("Resources/Map/wall.png");
	model.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/Map/wall.png");

	model.SetCamera(camera2.get());

	TextureManager::GetInstance()->LoadTexture("Resources/ao.png");
	title.Initialize({0,0}, {320,180}, "Resources/ao.png");
}

void TitleScene::Update() {
	camera2->Update();
	model.Update();

	title.Update();

	if (KeyInput::GetInstance()->PushKey(DIK_A)) {
		sceneNo = INGAME;
	}
}

void TitleScene::Draw() {
	model.Draw();
	title.Draw();
}