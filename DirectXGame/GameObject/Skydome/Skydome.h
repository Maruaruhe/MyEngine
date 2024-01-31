#pragma once
#include "../../Object/Model/Model.h"

class Skydome {
public:
	void Initialize(const std::string& filename);

	void Update();

	void Draw();

private:
	Transform transform;
	Model model_;
};
