#pragma once
#include "../../../../Object/Model/Model.h"

class IItem
{
public:
	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

private:
	Transform transform;
	Model model;

	int weight;
	bool isTaken;
};

