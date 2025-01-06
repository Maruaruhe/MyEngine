#pragma once
#include "../../../Object/Model/Model.h"
#include "../../../Object/Camera/Camera.h"
#include "../../../Object/AABB/AABB.h"

#include <vector>

class Player;
class MapJson;

//Enemy基底クラス
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

	void SetPlayer(Player* player) { player_ = player; }
	void SetMap(MapJson* map) { map_ = map; }

	bool isAlive_;
	bool isChase_;
	bool isAttacking_;

	Model model_;
	AABB collision_;
protected:
	Camera* camera_;

	MapJson* map_;
	Player* player_;
};

