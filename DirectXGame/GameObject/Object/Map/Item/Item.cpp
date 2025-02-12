#include "Item.h"

#include "../../../DirectXGame/Manager/TextureManager.h"
#include "../../../DirectXGame/Manager/ModelManager.h"
#include "../../../DirectXGame/Base/Input/Input.h"

using namespace MyEngine;

mapItem::mapItem(){}
mapItem::~mapItem(){}

void mapItem::Initialize(const Transform transform,  std::string filename) {
	ModelManager::GetInstance()->LoadModel("2x2cube");

	model_.Initialize("2x2cube");
	model_.transform = transform;

	TextureManager::GetInstance()->LoadTexture("Resources/Map/" + filename + ".png");
	model_.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/Map/" + filename + ".png");

	model_.material->enableLighting = true;

	nowWhere_ = INS;
	whereSlot = -1;
}

void mapItem::Update() {
	//所有されてるとき
	if (isTaken_) {
		isabletobetaken_ = false;
	}
	else { //所有されていないとき

	}
	model_.Update();
}

void mapItem::Draw() {
	model_.Draw();
}

void mapItem::TakenItem() {
	if (KeyInput::GetInstance()->TriggerKey(DIK_F)) {
		isTaken_ = true;
	}
}

void mapItem::DropItem(Vector3 player) {
	if (KeyInput::GetInstance()->TriggerKey(DIK_G)) {
		isTaken_ = false;
		model_.transform.translate = player;

	}
}