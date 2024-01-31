#include "TitleScene.h"

void TitleScene::Initialize(){
	input = Input::GetInstance();
	black.Initialize({-1280,0},{0,720});
	title.Initialize({ 0,0 }, { 1280,720 });

	scroll = 0;
	isScroll = false;

	sphere.Initialize();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	trans.translate = { 0,0,0 };
	trans.rotate = { 0,4.7f,0 };
	trans.scale = { 1,1,1 };
}

void TitleScene::Update(){
	if (input->TriggerKey(DIK_SPACE)) {
		isScroll = true;
	}
	if (isScroll) {
		scroll += scrollSpeed;
		trans.translate.y += 0.02f;
	}
	if (scroll >= 1280) {
		sceneNo = INGAME;
	}
	Vector2 scrollPosition = { float(scroll),0 };
	black.SetPosition(scrollPosition);
	title.Update();
	black.Update();

	DirectionalLight light;
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;
	Vector4 a = { 1,1,1,1 };

	sphere.Update(a, trans, camera->GetTransform(), light);
}

void TitleScene::Draw() {
	//title.Draw();
	black.Draw();
	sphere.Draw();
}