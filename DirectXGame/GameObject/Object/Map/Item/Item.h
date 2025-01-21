#pragma once
#include "../../../../Object/Model/Model.h"
#include "../../../../Object/Camera/Camera.h"

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

	bool isTaken_ = false;
	bool isabletobetaken_ = false;
	Model model_;

protected:
	Camera* camera_;

	int weight_;
};

