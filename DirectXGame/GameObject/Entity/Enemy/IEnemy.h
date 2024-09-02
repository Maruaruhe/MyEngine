#pragma once
#include "../../../Object/Model/Model.h"
#include "../../../Object/Camera/Camera.h"
#include "../../../Object/AABB/AABB.h"

#include <vector>

class Player;
class Map;

class IEnemy
{
public:
	virtual void Initialize(Transform transform);

	virtual void Update();

	virtual void Draw();

	bool isAlive;
	bool isChase;
	bool isAttacking;

	Model model;
	AABB collision;
protected:
	Camera* camera;

	Map* map;
	Player* player;
};

