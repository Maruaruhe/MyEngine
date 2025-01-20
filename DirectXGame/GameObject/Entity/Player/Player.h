#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Object/Sprite/Sprite.h"
#include "../../../Base/Input/Input.h"

#include "../../Object/Map/MapJson.h"
#include "../../../Object/SpotLight/SpotLight.h"

struct State {
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
	/// マップ関連
	/// </summary>

	/// <summary>
	/// 死亡時
	/// </summary>
	void DeathUpdate();
	int deathCount;

	//視線判定
	Vector3 CheckLineOfSightCollision();
	//アイテムのVector
	Vector3 GetItemFrontVector();

	//Item判定
	void CheckItemCollision();
	void CheckItemBring();

private:
	MyEngine::GamePadInput* pInput_;
	MyEngine::KeyInput* kInput_;

	//MapInfo
	MapJson* mapJson_;

	SpotLight sLight_;

	Sprite deads_;
	Sprite toggleLight_;
	Sprite holdQ_;

	bool canOpenDoor_;

	WASD wasd_;
	const float kXkeys = 50;
	const float kYkeys = 620;

	WASD arrows_;
	const float kXAkeys =1130;
	const float kYAkeys = 620;

	KeyState space_;
};