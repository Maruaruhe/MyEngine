#include "IItem.h"
#include "../../../../Base/Input/Input.h"

void IItem::Initialize(){}

void IItem::Update(){}

void IItem::Draw(){}

void IItem::TakenItem() {
	if (isabletobetaken && !isTaken) { // 取得可能状態
		if (KeyInput::GetInstance()->PushKey(DIK_F)) { //かつキーを押された場合
			isTaken = true;

			model.transform.scale *= 0.75f;;
		}
	}
}