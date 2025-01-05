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
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(Transform transform);

	/// <summary>
	/// Update
	/// </summary>
	virtual void Update();

	/// <summary>
	/// Draw
	/// </summary>
	virtual void Draw();

	bool isAlive_;
	bool isChase_;
	bool isAttacking_;

	Model model_;
	AABB collision_;
protected:
	Camera* camera_;

	Map* map_;
	Player* player_;
};

