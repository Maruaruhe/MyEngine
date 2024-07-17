#pragma once
#include "Wall/Wall.h"
#include "../../../Object/AABB/AABB.h"

enum BlockType {
	//NONE = 0,
	WALL = 1,
};

class Map
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	void CheckCollision(AABB pAABB, Vector2 move, Vector3* fixVector);

private:
	std::vector<std::vector<int>> LoadMapData(const std::string& filename);

	void CreateWall(Camera* camera);
	void CreateFloor(Camera* camera);
	void CreateRoof(Camera* camera);

private:
	const std::string directoryPath = "Resources/csv/";

	//Map情報
	std::vector<std::vector<int>> mapData;

	//Wallのリスト
	std::vector<Wall> walls;
	Wall w;
	Wall floor;
	Wall roof;

	//
	const float kMapHeight = 10.0f;
	const float dis = 0.01f;
};

