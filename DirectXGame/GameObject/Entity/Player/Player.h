#pragma once
#include <iostream>
#include <memory>

#include "../../../Base/Input/Input.h"
#include "../../../Object/Model/Model.h"

class Player
{
public:
	void Initialize(const std::string& filename);

	void Update(Vector4& color, const Transform& transform_, DirectionalLight& directionalLight);

	void Move();

	void Draw();

	Transform GetTransform() { return transform_; }
private:
	Input* input_ = nullptr;

	Transform transform_;
	std::unique_ptr<Model> model_;
};