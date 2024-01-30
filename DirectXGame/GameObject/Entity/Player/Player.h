#pragma once
#include <iostream>
#include <memory>
#include <list>

#include "../../../Math/Vector4.h"
#include "../../../Base/Input/Input.h"
#include "../../../Object/Model/Model.h"
#include "Bullet/Bullet.h"

class Player
{
public:
	void Initialize(const std::string& filename);

	void Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight);

	void Draw();

	std::list<Bullet*> GetBullets() { return bullets; }

	Transform GetTransform() { return transform_; }
	void SetTransform(Transform transform) { transform_ = transform; }

private:

	void Move();
	void ReticleMove();
	void BulletMove();

	void Attack();

private:
	Input* input_ = nullptr;

	Transform transform_;
	std::unique_ptr<Model> model_;

	Transform reticleTransform_;
	std::unique_ptr<Model> reticleModel_;

	std::list<Bullet*> bullets;

	bool isSee = false;
	Vector3 velocity;
	const float speed = 0.1f;

	const int kBulletMaxCoolTime = 30;
	int bulletCoolTime = 30;
};