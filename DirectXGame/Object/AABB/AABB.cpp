#include "AABB.h"
#include "../../Math/MathOperator.h"

bool AABB::CheckCollision(AABB a) {
	if ((min.x < a.max.x && max.x > a.min.x) && (min.y < a.max.y && max.y > a.min.y) &&
		(min.z < a.max.z && max.z > a.min.z)) {
		return true;
	}
	return false;
}

void AABB::CreateEntityAABB(Transform transform) {
	min = {
		-0.5f * transform.scale.x, -0.5f * transform.scale.y,
		-0.5f * transform.scale.z };
	max = {
		0.5f * transform.scale.x, 0.5f * transform.scale.y,
		0.5f * transform.scale.z };

	min.x += transform.translate.x;
	min.y += transform.translate.y;
	min.z += transform.translate.z;

	max.x += transform.translate.x;
	max.y += transform.translate.y;
	max.z += transform.translate.z;
}


void AABB::CreateModelAABB(Transform transform) {
	min = { 0.0f,0.0f,0.0f };
	max = {
		1.0f * transform.scale.x, 1.0f * transform.scale.y,
		1.0f * transform.scale.z };

	min.x += transform.translate.x;
	min.y += transform.translate.y;
	min.z += transform.translate.z;

	max.x += transform.translate.x;
	max.y += transform.translate.y;
	max.z += transform.translate.z;

	this->transform.scale = transform.scale;
	//mid
	this->transform.translate = { (min.x + max.x) / 2,(min.y + max.y) / 2 ,(min.z + max.z) / 2 };
}