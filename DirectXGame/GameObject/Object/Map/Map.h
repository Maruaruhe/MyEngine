#pragma once
#include "Wall/Wall.h"
#include "../../../Object/AABB/AABB.h"

#include "Item/SmallItem/SmallItem.h"

enum BlockType {
	//NONE = 0,
	WALL = 1,
};

class Map
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="camera"></param>
	void Initialize(Camera* camera);

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Draw
	/// </summary>
	void Draw();

	/// <summary>
	/// マップのロード
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	std::vector<std::vector<int>> LoadMapData(const std::string& filename);

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="pAABB"></param>
	/// <param name="move"></param>
	/// <param name="fixVector"></param>
	void CheckCollision(AABB pAABB, Vector3 move, Vector3* fixVector);
	bool CheckCollisionWithFloor(AABB pAABB, Vector3 move, Vector3* fixVector);

	//Getter
	SmallItem* GetItem() { return sItem.get(); }
	std::vector<Wall> GetWall() { return walls_; }

	//Map情報
	std::vector<std::vector<int>> mapData;

private:

	//Mapの生成
	void CreateWall(Camera* camera);
	void CreateFloor(Camera* camera);
	void CreateRoof(Camera* camera);
	void CreateModels(Camera* camera);
	void CreateItem(Camera* camera);

private:
	const std::string directoryPath = "Resources/csv/";



	//Wallのリスト
	std::vector<Wall> walls_;

	//仮------------------------------------------------------------------------
	Wall w;
	Wall floor;
	Wall roof;
	Model mapModel;
	Model dish;
	//---------------------------------------------------------------------------

	std::unique_ptr<SmallItem> sItem;

	Camera* camera_;

	//
	const float kMapHeight = 10.0f;
	const float dis = 0.01f;
};

