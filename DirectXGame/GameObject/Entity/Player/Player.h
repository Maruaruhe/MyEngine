#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Base/Input/Input.h"

#include "../../Object/Map/Map.h"
#include "../../../Object/SpotLight/SpotLight.h"

struct State {
	float kMapSpeed; //移動速度制限
	float moveSpeed; //移動速度
	float stamina; //スタミナ
	bool isDash; //ダッシュ状態
	bool isJump; //ジャンプ状態
	float weight; //重量
	float HP; //体力

};

class Player {
public:

	void Initialize();

	void Update();
	void LightUpdate();

	void Draw();

	void SetMap(Map* map) { this->map = map; }

	Model model;
	Model view;

	Vector3 GetFrontVector(float length);
	Vector3 GetFrontLightVector(float length);

	AABB GetCollision();
private:
	void Move();
	Vector3 CheckLineOfSightCollision();
	Vector3 GetItemFrontVector();

	void CheckItemCollision();
	void CheckItemBring();

private:
	GamePadInput* pInput;
	KeyInput* kInput;

	//MapInfo
	Map* map;
	SpotLight sLight;
};