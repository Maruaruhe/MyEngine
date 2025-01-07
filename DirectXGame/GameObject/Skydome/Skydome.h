#pragma once
#include "../../Object/Model/Model.h"

class Skydome {
public:
	void Initialize(const std::string& filename);

	void Update();

	void Draw();

	std::unique_ptr<Model> model;
private:
};
