#include "ClearScene.h"
void ClearScene::Initialize() {
	input = Input::GetInstance();
	sphere.Initialize();

	camera = std::make_unique<Camera>();
	camera->Initialize();

	trans.translate = { 0,0,0 };
	trans.rotate = { 0,4.7f,0 };
	trans.scale = { 1,1,1 };
}

void ClearScene::Update() {
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
	}
	DirectionalLight light;
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;
	Vector4 a = { 1,1,1,1 };

	trans.rotate.y += 0.02f;

	sphere.Update(a, trans, camera->GetTransform(), light);
}

void ClearScene::Draw() {
	sphere.Draw();
}