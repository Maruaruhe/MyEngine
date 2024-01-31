#pragma once
#include <iostream>
#include <memory>

#include "../../../../Math/Vector4.h"
#include "../../../../Object/Sphere/Sphere.h"

class Bullet
{
public:
	void Initialize(Vector3& position, Vector3& velocity);

	void Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight);

	void Move();

	void Draw();

	Transform GetTransform() { return transform; }
	Vector3 GetPosition() { return transform.translate; }
	void SetTransform(Transform transform) { transform = transform; }
	void SetPosition(Vector3& position) { transform.translate = position; }

	void SetIsAlive(bool a) { isActive = a; }

	bool GetIsActive() { return isActive; }

private:
	bool isActive = true;

	Transform transform;
	Sphere bullet;

	Vector3 velocity;

	int deathTimer = lifeTime;
	const int lifeTime = 60 * 1;
};