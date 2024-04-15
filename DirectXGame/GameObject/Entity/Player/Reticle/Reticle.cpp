#include "Reticle.h"
#include "../../../../Manager/ModelManager.h"
#include "../../../../Base/Input/Input.h"

void Reticle::Initialize() {
	ModelManager::GetInstance()->LoadModel("bullet");
	model = std::make_unique<Model>();
	model->Initialize("bullet");

	model->transform.translate.z = 35.0f;
}

void Reticle::Update() {
	Move();
	model->Update();
}

void Reticle::Draw() {
	model->Draw();
}

void Reticle::Move() {
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		model->transform.translate.y += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		model->transform.translate.y -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		model->transform.translate.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		model->transform.translate.x += 0.1f;
	}
}