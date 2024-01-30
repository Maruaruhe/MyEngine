#pragma once
#include <iostream>
#include <memory>

#include "../../../Math/Vector4.h"
#include "../../../Base/Input/Input.h"
#include "../../../Object/Model/Model.h"
#include "../../../Object/Sphere/Sphere.h"
#include "Bullet/Bullet.h"

class Player
{
public:
	void Initialize(const std::string& filename);

	void Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight);

	void Move();
	void ReticleMove();
	void BulletMove();

	void Attack();

	void Draw();

	Transform GetTransform() { return transform_; }
	Transform GetBulletTransform() { return bulletTransform_; }
	void SetTransform(Transform transform) { transform_ = transform; }

private:
	Input* input_ = nullptr;

	Transform transform_;
	std::unique_ptr<Model> model_;

	Transform reticleTransform_;
	std::unique_ptr<Model> reticleModel_;

	Transform bulletTransform_;
	std::unique_ptr<Model> bulletModel_;

	Transform bu;
	std::unique_ptr<Sphere> bullet;

	bool isSee = false;
	Vector3 velocity;
	//std::unique_ptr<Bullet> bullet_;

	const int kBulletMaxCoolTime = 30;
	int bulletCoolTime = 0;
};