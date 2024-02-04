#include "TitleScene.h"

void TitleScene::Initialize(){
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;

	color = { 1.0f,1.0f,1.0f,1.0f };

	transform.rotate = {};
	transform.scale = { 1,1,1 };
	transform.translate = { 0,0,0 };


	sphere.Initialize();
	camera.Initialize();
}

void TitleScene::Update(){
	sphere.Update(color, transform, camera.GetTransform(), light);
}

void TitleScene::Draw() {
	sphere.Draw();
}