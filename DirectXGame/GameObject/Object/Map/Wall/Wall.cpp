#include "Wall.h"
#include "../../../DirectXGame/Manager/TextureManager.h"

using namespace MyEngine;

void Wall::Initialize(const Transform transform, const Direction direction, std::string filename) {
	ModelManager::GetInstance()->LoadModel("2x2cube");
	TextureManager::GetInstance()->LoadTexture("Resources/Map/wall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/" +  filename + ".png");

	model_.Initialize("2x2cube");
	model_.transform = transform;
	direction_ = direction;

	model_.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/Map/wall.png");
	model_.material->enableLighting = true;
}

void Wall::Update() {
	model_.Update();
}

void Wall::Draw() {
	model_.Draw();
}