#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Object/Sprite/Sprite.h"
#include "../../../Base/Input/Input.h"

#include "../../Object/Map/Map.h"
#include "../../Object/Map/MapJson.h"
#include "../../../Object/SpotLight/SpotLight.h"

struct State {
	float kMapSpeed; //移動速度制限
	float moveSpeed; //移動速度
	float stamina; //スタミナ
	bool isDash; //ダッシュ状態
	bool isJump; //ジャンプ状態
	float weight; //重量
	float HP; //体力
	Vector3 velocity;
	bool isUsingLight;
	bool isAlive;
};

class Player {
public:

	void Initialize();

	void Update();
	void LightUpdate();

	void Draw();

	void SetMap(Map* map) { this->map_ = map; }
	void SetMap(MapJson* map) { this->mapJson_ = map; }

	Model model;
	Model deadModel;
	Model view;
	State state_;

	Transform tForCamera;
	Transform deadCamera;
	int deadFlame;
	int aliveFrame;

	Vector3 GetFrontVector(float length);
	Vector3 GetFrontLightVector(float length);

	AABB GetCollision();
private:
	void Move();
	void Jump();

	void DeathUpdate();
	int deathCount;

	Vector3 CheckLineOfSightCollision();
	Vector3 GetItemFrontVector();

	void CheckItemCollision();
	void CheckItemBring();

private:
	GamePadInput* pInput;
	KeyInput* kInput;

	//MapInfo
	Map* map_;
	MapJson* mapJson_;

	SpotLight sLight;

	Sprite deads;
};