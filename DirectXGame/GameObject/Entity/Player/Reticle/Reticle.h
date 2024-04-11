#pragma once
#include "../../../../Object/Model/Model.h"

class Reticle
{
public:
	void Initialize();
	void Update();
	void Draw();

	void Move();

	Vector3 GetPosition() { return model->transform.translate; }

	std::unique_ptr<Model> model;
};

