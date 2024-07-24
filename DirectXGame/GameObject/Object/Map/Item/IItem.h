#pragma once
#include "../../../../Object/Model/Model.h"
#include "../../../../Object/Camera/Camera.h"

class IItem
{
public:
	virtual void Initialize(Transform transform, Camera* camera);

	virtual void Update();

	virtual void Draw();

	bool isTaken = false;
	Model model;
protected:
	Transform transform;
	Camera* camera;

	int weight;
	bool isabletobetaken;
};

