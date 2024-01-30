#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"

#include "../Object/Sprite/Sprite.h"

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	Input* input = nullptr;
	Sprite black;
	Sprite title;

	int scroll;
	const int scrollSpeed = 8;
	bool isScroll;
};

