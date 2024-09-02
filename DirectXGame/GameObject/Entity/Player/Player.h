#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Base/Input/Input.h"

#include "../../Object/Map/Map.h"
#include "../../../Object/SpotLight/SpotLight.h"

class Player {
public:

	void Initialize();

	void Update();

	void Draw();

	void SetMap(Map* map) { this->map = map; }

	Model model;
	Model view;

private:
	void Move();
	AABB GetCollision();
	Vector3 CheckLineOfSightCollision();
	Vector3 GetFrontVector(float length);
	Vector3 GetItemFrontVector();

	void CheckItemCollision();
	void CheckItemBring();

private:
	GamePadInput* pInput;
	KeyInput* kInput;

	//MapInfo
	Map* map;
};