#pragma once
#include "Wall/Wall.h"

enum BlockType {
	NONE = 0,
	WALL = 1,
};

class Map
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	std::vector<std::vector<int>> LoadMapData(const std::string& filename);

	void CreateWall();

private:
	const std::string directoryPath = "Resources/csv/";

	//Map情報
	std::vector<std::vector<int>> mapData;

	//Wallのリスト
	std::vector<Wall> walls;

	//
	const float kMapHeight = 10.0f;
};

