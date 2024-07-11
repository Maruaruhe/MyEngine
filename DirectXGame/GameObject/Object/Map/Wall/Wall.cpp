#include "Wall.h"

void Wall::Initialize(const Vector3 pos, const Vector3 scale) {
	ModelManager::GetInstance()->LoadModel("box");
	model.Initialize("box");
	model.transform.translate = pos;
	model.transform.scale = scale;

	model.material->uvTransform.m[0][0] = scale.y;
	if (scale.x != 1.0f) {
		model.material->uvTransform.m[1][1] = scale.x;
	}
	else {
		model.material->uvTransform.m[1][1] = scale.z;
	}
}

void Wall::Update() {
	model.Update();
}

void Wall::Draw() {
	model.Draw();
}