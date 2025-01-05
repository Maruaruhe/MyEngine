#pragma once
#include "../../../../Object/Model/Model.h"
#include "../../../../Object/Camera/Camera.h"

class IItem
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// Update
	/// </summary>
	virtual void Update();

	/// <summary>
	/// Drtaw
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 取得状況
	/// </summary>
	virtual void TakenItem();

	bool isTaken_ = false;
	bool isabletobetaken_ = false;
	Model model_;
protected:
	Camera* camera_;

	int weight_;
};

