#pragma once
#include <iostream>
#include <memory>

#include "../../../../Math/Vector4.h"
#include "../../../../Object/Model/Model.h"
#include "../../../../Object/Sphere/Sphere.h"

class Bullet
{
public:
	void Initialize(const std::string& filename, Vector3& position, Vector3& velocity);

	void Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight);

	void Move();

	void Draw();

	Transform GetTransform() { return transform_; }
	void SetTransform(Transform transform) { transform_ = transform; }
	void SetPosition(Vector3& position) { transform_.translate = position; }

	void SetIsAlive(bool a) { isDead_ = a; }
	bool GetIsAlive() { return isDead_; }

private:

	Transform transform_;
	std::unique_ptr<Model> model_;
	//std::unique_ptr<Sphere> ball_;

	bool isDead_ = false;
	Vector3 velocity_;
};