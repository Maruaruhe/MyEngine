#include "Wall.h"
#include "../../../DirectXGame/Manager/TextureManager.h"

void Wall::Initialize(const Vector3 pos, const Vector3 scale) {
	ModelManager::GetInstance()->LoadModel("Map/wall");
	model_.Initialize("Map/wall");
	model_.transform.translate = pos;
	model_.transform.scale = scale;

	TextureManager::GetInstance()->LoadTexture("Resources/Map/wall.png");
	model_.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/Map/wall.png");

	//model.material->uvTransform.m[0][0] = scale.y;
	//if (scale.x != 1.0f) {
	//	model.material->uvTransform.m[1][1] = scale.x;
	//}
	//else {
	//	model.material->uvTransform.m[1][1] = scale.z;
	//}
}

void Wall::Update() {
	model_.Update();
}

void Wall::Draw() {
	model_.Draw();
}