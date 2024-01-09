#pragma once
#include <iostream>
#include <memory>

#include "../../../Base/Input/Input.h"
#include "../../../Object/Model/Model.h"

class Player
{
public:
	void Initialize(DirectX12* directX12, const std::string& filename);

	void Update(Vector4& color, DirectionalLight& directionalLight);

	void Move();

	void Draw();

	Transform GetTransform() { return transform_; }
	void SetTransform(Transform transform) { transform_ = transform; }

private:
	Input* input_ = nullptr;
	DirectX12* directX12_;

	Transform transform_;
	std::unique_ptr<Model> model_;
	//Model* model_;
};