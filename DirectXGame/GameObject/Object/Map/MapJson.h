#pragma once
#include "../../../Manager/FileManager.h"
#include "../../../Object/AABB/AABB.h"

//JsonMapクラス
class MapJson
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="camera"></param>
	void Initialize(Camera* camera, std::string filename);

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
	void CheckCollisionFloor(AABB pAABB, Vector3 move, Vector3* fixVector, bool* isFloor);
	bool CheckCollisionWithEye(const Segment& eyeSegment);

private:

private:
	const std::string directoryPath = "Resources/csv/";

	Camera* camera_;

	LevelData level_;

	//
	const float kMapHeight = 10.0f;
	const float dis = 0.01f;
};

