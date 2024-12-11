#pragma once
#include "../../../Manager/FileManager.h"
#include "../../../Object/AABB/AABB.h"

class MapJson
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	void CheckCollision(AABB pAABB, Vector3 move, Vector3* fixVector);
	void CheckCollisionFloor(AABB pAABB, Vector3 move, Vector3* fixVector, bool* isFloor);

private:

private:
	const std::string directoryPath = "Resources/csv/";

	Camera* camera_;

	LevelData level;

	//
	const float kMapHeight = 10.0f;
	const float dis = 0.01f;
};

