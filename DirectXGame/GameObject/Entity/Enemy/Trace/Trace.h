#pragma once
#include "../IEnemy.h"

class Wall;

//TraceEnemyクラス
class Trace : public IEnemy
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="transform"></param>
	void Initialize(Transform transform);
	/// <summary>
	/// Update
	/// </summary>
	void Update();
	/// <summary>
	/// Draw
	/// </summary>
	void Draw();
private:
	/// <summary>
	/// プレイヤー索敵
	/// </summary>
	void FindPlayer();
	/// <summary>
	/// プレイヤー追跡
	/// </summary>
	void ChasePlayer();

	std::vector<Wall> walls_;
	Vector3 velocity_{};
	Vector3 distance_{};

	int tracingTime_;

	float maxSpeed_ = 0.20f;
	float firstSpeed_ = 0.05f;
	float chaseSpeed_;
};

