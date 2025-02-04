#pragma once
#include "../../../Manager/FileManager.h"
#include "../../../Object/AABB/AABB.h"

#include "Wall/Wall.h"
#include "Door/Door.h"
#include "Item/Item.h"
#include "../../Entity/Enemy/Trace/Trace.h"
#include "../../Entity/Player/Player.h"

//JsonMapクラス
class MapJson
{
public:
	MapJson();
	~MapJson();
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="camera"></param>
	void Initialize(Camera* camera, std::string filename, Player* player);

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Draw
	/// </summary>
	void Draw();

	//当たり判定
	void CheckCollision(AABB pAABB, Vector3 move, Vector3* fixVector);
	void CheckCollisionFloor(AABB pAABB, Vector3* move, Vector3* fixVector, bool* isFloor);
	bool CheckCollisionWithEye(const Segment& eyeSegment);

	LevelData GetLevel() { return level_; }
	std::vector<Wall> GetWalls() { return walls_; }

	//GameSceneに持たせる
	std::vector<std::shared_ptr<mapItem>> CreateItem(Camera* camera, std::string filename);
private:

	void CreateWall(Camera* camera);
	void CreateWayPoint(Camera* camera);
	void CreateDoor(Camera* camera);
	void CreateEnemy(Camera* camera, Player* player);

private:
	const std::string directoryPath = "Resources/csv/";

	Camera* camera_;
	Player* player_;

	LevelData level_;
	std::vector<Wall> walls_;
	std::vector<Door> doors_;
	std::vector<Trace> traces_;
	//std::vector<mapItem*> items_;

	//
	const float kMapHeight = 10.0f;
	const float dis = 0.01f;
};

