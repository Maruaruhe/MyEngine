#pragma once
#include <iostream>
#include <memory>
#include <list>

#include "../../../Base/Input/Input.h"
#include "../../../Base/GlobalVariables/GlobalVariables.h"

#include "../../../Math/Vector4.h"
#include "../../../Object/Model/Model.h"
#include "Bullet/Bullet.h"

class Player
{
public:
	void Initialize();

	void Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight);

	void Draw();

	std::list<Bullet*> GetBullets() { return bullets; }

	Transform GetTransform() { return transform_; }
	void SetTransform(Transform transform) { transform_ = transform; }

private:

	void Move();
	void ReticleMove();

	void ApplyGlobalVariables();

	void Attack();

private:
	Input* input_ = nullptr;

	Transform transform_;
	Transform armTransform_;

	std::unique_ptr<Model> body;
	std::unique_ptr<Model> head;
	std::unique_ptr<Model> left;
	std::unique_ptr<Model> right;

	Transform reticleTransform_;
	std::unique_ptr<Model> reticleModel_;

	std::list<Bullet*> bullets;

	bool isSee = false;
	Vector3 velocity;
	const float speed = 0.1f;

	const int kBulletMaxCoolTime = 30;
	int bulletCoolTime = 30;
};