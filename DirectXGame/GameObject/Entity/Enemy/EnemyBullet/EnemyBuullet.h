#pragma once
#include "../../../../Object/Model/Model.h"
#include "../../../../Math/MathOperator.h"

class Camera;

class EnemyBullet
{
public:
	void Initialize(const Vector3& pos, const Vector3& velo, Camera* camera);

	void Update();

	void Draw();

	bool isDead() const { return isDead_; }

	void OnCollision();

	Vector3 GetWorldPosition() { return model.transform.translate; }

	Model model;
private:
	Vector3 velocity;

	static const int32_t kLifeTime = 60 * 2;
	int32_t deathTimer = kLifeTime;
	bool isDead_ = false;

};

