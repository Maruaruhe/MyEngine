#include "AABB.h"
#include <algorithm>
#include <minwindef.h>
#include "../../Math/MathOperator.h"

void AABB::Initialize() {

}

bool AABB::CheckCollision(AABB a) {
	if ((min.x < a.max.x && max.x > a.min.x) && (min.y < a.max.y && max.y > a.min.y) &&
		(min.z < a.max.z && max.z > a.min.z)) {
		return true;
	}
	return false;
}

void AABB::CreateAABB(Transform transform) {
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

//bool AABB::CheckLineCollision(const Segment& segment) {
//
//	//x
//	float txMin = (min.x - segment.start.x) / (segment.end.x - segment.start.x);
//	float txMax = (max.x - segment.start.x) / (segment.end.x - segment.start.x);
//	float tNearX = min(txMin, txMax);
//	float tFarX = max(txMin, txMax);
//
//	//y
//	float tyMin = (min.y - segment.start.y) / (segment.end.y - segment.start.y);
//	float tyMax = (max.y - segment.start.y) / (segment.end.y - segment.start.y);
//	float tNearY = min(tyMin, tyMax);
//	float tFarY = max(tyMin, tyMax);
//
//
//	//z
//	float tzMin = (min.z - segment.start.z) / (segment.end.z - segment.start.z);
//	float tzMax = (max.z - segment.start.z) / (segment.end.z - segment.start.z);
//	float tNearZ = min(tzMin, tzMax);
//	float tFarZ = max(tzMin, tzMax);
//
//	float tMin = max(max(tNearX, tNearY), tNearZ);
//	float tMax = min(min(tFarX, tFarY), tFarZ);
//
//	if (tMin <= tMax) {
//		if (tMin <= 1.0f && 0.0f <= tMax) {
//			return true;
//		}
//	}
//	return false;
//}

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

//Vector3 AABB::NearestPoint(const Segment& segment) {
//	Vector3 m = (segment.start + segment.end) * 0.5f;
//
//	// AABBの中心を基準にした相対的な線分の位置
//	Vector3 mToAABB = m - Vector3((min.x + max.x) * 0.5f,
//		(min.y + max.y) * 0.5f,
//		(min.z + max.z) * 0.5f);
//
//	// AABBの半分のサイズ
//	Vector3 aabbHalfSize = Vector3((max.x - min.x) * 0.5f,
//		(max.y - min.y) * 0.5f,
//		(max.z - min.z) * 0.5f);
//
//	// 各軸ごとに最近接点を計算
//	float distX = std::clamp(mToAABB.x, -aabbHalfSize.x, aabbHalfSize.x);
//	float distY = std::clamp(mToAABB.y, -aabbHalfSize.y, aabbHalfSize.y);
//	float distZ = std::clamp(mToAABB.z, -aabbHalfSize.z, aabbHalfSize.z);
//
//	// 最終的な最近接点を計算
//	return Vector3((min.x + max.x) * 0.5f + distX,
//		(min.y + max.y) * 0.5f + distY,
//		(min.z + max.z) * 0.5f + distZ);
//}

//bool CheckLineCollision(Segment a, Segment b) {
//	a.start.y = 1.0f;
//	a.end.y = 1.0f;
//	b.start.y = 1.0f;
//	b.end.y = 1.0f;
//
//	Vector3 startA = a.start;
//	Vector3 vectorA = a.end - a.start;
//
//	Vector3 startB = b.start;
//	Vector3 vectorB = b.end - b.start;
//
//	Vector3 v = startB - startA;
//	float crossv1v2 = CrossProduct(vectorA, vectorB);
//	if (crossv1v2 == 0.0f) {
//		return false;
//	}
//	float crossv1 = CrossProduct(v, vectorA);
//	float crossv2 = CrossProduct(v, vectorB);
//
//	float t1 = crossv2 / crossv1v2;
//	float t2 = crossv1 / crossv1v2;
//
//	const float eps = 0.00001f;
//	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
//		// 交差していない
//		return false;
//	}
//	return true;
//}
//
//float CrossProduct(Vector3 a, Vector3 b) {
//	return a.x * b.z - a.z * b.x;
//}