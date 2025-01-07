#include "SmallItem.h"
#include "../../../../../Manager/ModelManager.h"
#include "../../../../../Manager/TextureManager.h"

using namespace MyEngine;

void SmallItem::Initialize(Transform transform, Camera* camera) {
	ModelManager::GetInstance()->LoadModel("Map/Item/smallItem");
	model_.Initialize("Map/Item/smallItem");
	TextureManager::GetInstance()->LoadTexture("Resources/pizza.png");
	model_.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/pizza.png");
	model_.SetCamera(camera);
	model_.transform = transform;
}

void SmallItem::Update() {
	model_.Update();
}

void SmallItem::Draw() {
	model_.Draw();
}

void SmallItem::CheckLineCollision(Segment segment) {
	//アイテムのAABBを生成
	AABB aabb;
	aabb.CreateEntityAABB(model_.transform);

	//線分との当たり判定チェック
	if (aabb.CheckLineCollision(segment)) {
		isabletobetaken_ = true;
	}

}