#include "Bullet.h"

void Bullet::Initialize(Vector3& position, Vector3& velocity) {
	bullet.Initialize();

	isActive = true;
	transform.translate = position;
	transform.rotate = { 0.0f,0.0f,0.0f };
	transform.scale = { 0.3f,0.3f,2.0f };

	this->velocity = velocity;
}

void Bullet::Update(Vector4& color, const Transform& cameraTransform, DirectionalLight& directionalLight) {
	Move();
	bullet.Update(color, transform, cameraTransform, directionalLight);
	if (--deathTimer <= 0) {
		isActive = false;
	}
}

void Bullet::Move() {
	transform.translate.x += velocity.x;
	transform.translate.y += velocity.y;
	transform.translate.z += velocity.z;
}

void Bullet::Draw() {
	bullet.Draw();
}