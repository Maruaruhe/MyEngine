#pragma once
#include "../../../../Object/Model/Model.h"
#include "../../../../Object/Camera/Camera.h"

class IItem
{
public:
	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	virtual void TakenItem();

	bool isTaken = false;
	bool isabletobetaken = false;
	Model model;
protected:
	Camera* camera_;

	int weight;
};

