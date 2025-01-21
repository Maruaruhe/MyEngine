#include "Item.h"

#include "../../../DirectXGame/Manager/TextureManager.h"
#include "../../../DirectXGame/Manager/ModelManager.h"

using namespace MyEngine;

void mapItem::Initialize(const Transform transform,  std::string filename) {
	ModelManager::GetInstance()->LoadModel("2x2cube");

	model_.Initialize("2x2cube");
	model_.transform = transform;

	TextureManager::GetInstance()->LoadTexture("Resources/Map/" + filename + ".png");
	model_.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/Map/" + filename + ".png");

	model_.material->enableLighting = true;
}

void mapItem::Update() {
	model_.Update();
}

void mapItem::Draw() {
	model_.Draw();
}