#pragma once
#include "IScene.h"

#include "../Base/Input/Input.h"

class ClearScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	Input* input = nullptr;
};

