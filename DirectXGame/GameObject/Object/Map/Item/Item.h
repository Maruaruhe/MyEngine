#pragma once
#include "../../../../Object/Model/Model.h"
#include "../../../../Object/Camera/Camera.h"

enum nowWhere {
	SH = 1,
	OU = 2,
	INS = 3,
};

class mapItem
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	void Initialize(const Transform transform, std::string filename);

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Drtaw
	/// </summary>
	void Draw();

	/// <summary>
	/// 取得状況
	/// </summary>
	void TakenItem();

	void DropItem(Vector3 player);

	bool isTaken_ = false;
	bool isabletobetaken_ = false;
	Model model_;
	nowWhere nowWhere_;


private:
	Camera* camera_;

	int weight_;
};

