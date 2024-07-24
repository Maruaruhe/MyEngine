#include "SmallItem.h"
#include "../../../../../Manager/ModelManager.h"

void SmallItem::Initialize(Transform transform, Camera* camera) {
	ModelManager::GetInstance()->LoadModel("Map/Item/smallItem");
	model.Initialize("Map/Item/smallItem");
	model.SetCamera(camera);
	model.transform = transform;
}

void SmallItem::Update() {
	model.Update();
}

void SmallItem::Draw() {
	model.Draw();
}

void SmallItem::CheckLineCollision(Segment segment) {
	//アイテムのAABBを生成
	AABB aabb;
	aabb.CreateEntityAABB(model.transform);

	//線分との当たり判定チェック
	if (aabb.CheckLineCollision(segment)) {
		isabletobetaken = true;
	}

}