#include "TitleScene.h"

void TitleScene::Initialize(){
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;

	cameraTransform.rotate = {};
	cameraTransform.scale = { 1,1,1 };
	cameraTransform.translate = { 0,0,-5 };

	color = { 1.0f,1.0f,1.0f,1.0f };

	transform.rotate = {};
	transform.scale = { 1,1,1 };
	transform.translate = { 0,0,0 };

	cameraFor = { 0.0f,0.0f,-5.0f };


	sphere.Initialize();
	camera.Initialize();
	model.Initialize("ghostPori");
}

void TitleScene::Update(){
	ImGui::Begin("LIGHT");
	ImGui::SliderFloat3("light.color", &light.color.x,0.0f,1.0f);
	ImGui::SliderFloat3("light.direction", &light.direction.x,-1.0f,1.0f);
	light.direction = Normalize(light.direction);
	ImGui::DragFloat("light.intensity", &light.intensity);
	ImGui::DragFloat3("cameraTransform", &cameraTransform.translate.x);
	ImGui::End();
	sphere.Update(color, transform, cameraTransform, light);
	model.Update(color, transform, cameraTransform, light);
}

void TitleScene::Draw() {
	sphere.Draw();
	//model.Draw();
}