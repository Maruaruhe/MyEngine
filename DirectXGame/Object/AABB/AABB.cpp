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

bool AABB::CheckLineCollision(const Segment& segment) {

	//x
	float txMin = (min.x - segment.start.x) / (segment.end.x - segment.start.x);
	float txMax = (max.x - segment.start.x) / (segment.end.x - segment.start.x);
	float tNearX = Min(txMin, txMax);
	float tFarX = Max(txMin, txMax);

	//y
	float tyMin = (min.y - segment.start.y) / (segment.end.y - segment.start.y);
	float tyMax = (max.y - segment.start.y) / (segment.end.y - segment.start.y);
	float tNearY = Min(tyMin, tyMax);
	float tFarY = Max(tyMin, tyMax);


	//z
	float tzMin = (min.z - segment.start.z) / (segment.end.z - segment.start.z);
	float tzMax = (max.z - segment.start.z) / (segment.end.z - segment.start.z);
	float tNearZ = Min(tzMin, tzMax);
	float tFarZ = Max(tzMin, tzMax);

	float tMin = Max(Max(tNearX, tNearY), tNearZ);
	float tMax = Min(Min(tFarX, tFarY), tFarZ);

	if (tMin <= tMax) {
		if (tMin <= 1.0f && 0.0f <= tMax) {
			return true;
		}
	}
	return false;
}

int AABB::Min(int num1, int num2) {
	if (num1 < num2) {
		return num1;
	}
	else {
		return num2;
	}

	return 0;
}

float AABB::Min(float num1, float num2) {
	if (num1 < num2) {
		return num1;
	}
	else {
		return num2;
	}
}

int AABB::Max(int num1, int num2) {
	if (num1 > num2) {
		return num1;
	}
	else {
		return num2;
	}
}

float AABB::Max(float num1, float num2) {
	if (num1 > num2) {
		return num1;
	}
	else {
		return num2;
	}
}