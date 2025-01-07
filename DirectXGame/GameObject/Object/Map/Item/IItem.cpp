#include "IItem.h"
#include "../../../../Base/Input/Input.h"

using namespace MyEngine;

void IItem::Initialize(){}

void IItem::Update(){}

void IItem::Draw(){}

void IItem::TakenItem() {
	if (isabletobetaken_ && !isTaken_) { // 取得可能状態
		if (KeyInput::GetInstance()->PushKey(DIK_F)) { //かつキーを押された場合
			isTaken_ = true;

			model_.transform.scale *= 0.75f;;
		}
	}
}