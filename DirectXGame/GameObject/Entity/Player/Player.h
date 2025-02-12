#pragma once
#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Object/Sprite/Sprite.h"
#include "../../../Base/Input/Input.h"

#include "../../Object/Map/MapJson.h"
#include "../../../Object/SpotLight/SpotLight.h"

struct ItemSlot {
	bool isFilled;//埋まっているか
	bool isSelected;//選択されているか
	Sprite sprite[2];
};

struct State {
	ItemSlot itemSlot[4]; //アイテム枠
	float kMapSpeed; //移動速度制限
	float moveSpeed; //移動速度
	float stamina; //スタミナ
	bool isDash; //ダッシュ状態
	bool isJump; //ジャンプ状態
	bool onFloor; //地面か空中か
	float weight; //重量
	float HP; //体力
	Vector3 velocity;
	bool isUsingLight;
	bool isAlive;
};

struct KeyState {
	Sprite sprite[2];
	bool isUsed;
};

struct WASD {
	Sprite w[2];
	Sprite a[2];
	Sprite s[2];
	Sprite d[2];
	bool isw;
	bool isa;
	bool iss;
	bool isd;
};

//プレイヤークラス
class Player {
public:
	/// <summary>
	/// Initialize
	/// </summary>
	void Initialize();

	/// <summary>
	/// Update
	/// </summary>
	void Update();
	void LightUpdate();

	/// <summary>
	/// Draw
	/// </summary>
	void Draw();

	/// <summary>
	/// Draw
	/// </summary>
	void SpriteDraw();

	//Setter
	void SetMap(MapJson* map) { this->mapJson_ = map; }
	void SetItems(std::vector<std::shared_ptr<mapItem>> items) { items_ = items; }

	Model model;
	Model deadModel;
	Model view;
	State state_;

	Transform tForCamera;
	Transform deadCamera;
	int deadFlame;
	int aliveFrame;

	//Getter
	Vector3 GetFrontVector(float length);
	Vector3 GetFrontLightVector(float length);
	bool StageChangeByDoor();
	bool GetIsAlive() { return state_.isAlive; }

	AABB GetCollision();
private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();
	/// <summary>
	/// アイテムバー更新処理
	/// </summary>
	void ItemSlotUpdate();
	/// <summary>
	/// マップ関連
	/// </summary>

	/// <summary>
	/// 死亡時
	/// </summary>
	void DeathUpdate();
	void DeathDraw();
	int deathCount;

	//視線判定
	Vector3 CheckLineOfSightCollision();
	//アイテムのVector
	Vector3 GetItemFrontVector();

	//Item判定
	void CheckItemCollision();
	void CheckItemBring(std::shared_ptr<mapItem> item);

	int CheckItemSlot();
	int GetNowSlot();

private:
	MyEngine::GamePadInput* pInput_;
	MyEngine::KeyInput* kInput_;

	//MapInfo
	MapJson* mapJson_;
	std::vector<std::shared_ptr<mapItem>> items_;

	SpotLight sLight_;

	Sprite deads_;
	Sprite toggleLight_;
	Sprite holdQ_;

	Sprite takeItem_;
	Sprite dropItem_;

	bool canOpenDoor_;
	bool canTakeItem_;
	bool canDropItem_;

	WASD wasd_;
	const float kXkeys = 50;
	const float kYkeys = 620;

	WASD arrows_;
	const float kXAkeys =1130;
	const float kYAkeys = 620;

	KeyState space_;
};