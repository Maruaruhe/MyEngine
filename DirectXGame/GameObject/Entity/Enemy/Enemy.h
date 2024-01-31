#pragma once
#include <iostream>
#include <memory>
#include "EnemyState.h"

#include "../../../Math/Vector4.h"
#include "../../../Base/Input/Input.h"
#include "../../../Object/Model/Model.h"

class BaseEnemyState;
class Enemy
{
public:
	void Initialize(const std::string& filename);

	void Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight);

	void Move();
	void ReticleMove();
	void BulletMove();

	void Attack();
	void Move(const Vector3& velocity);

	void Draw();

	void ChangeState(BaseEnemyState* newState);

	Transform GetTransform() { return transform_; }
	void SetTransform(Transform transform) { transform_ = transform; }
	void SetPosition(Vector3& transform) { transform_.translate = transform; }

	void SetLife(int a) { Life = a; }
	void SetIsAlive(bool a) { isAlive = a; }
	bool GetIsAlive() { return isAlive; }
	int GetLife() { return Life; }

private:
	Input* input_ = nullptr;

	Transform transform_;
	std::unique_ptr<Model> model_;


	bool isAlive = true;
	int Life = 5;
	int kMaxLife = 5;
	//std::unique_ptr<Bullet> bullet_;

	const int kBulletMaxCoolTime = 30;
	int bulletCoolTime = 0;

	BaseEnemyState* state;
};