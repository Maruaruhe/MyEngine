#pragma once
#include "../Base/Input/Input.h"

enum SCENE {
	TITLE = 0,
	INGAME = 1,
	CLEAR = 2,
	FAULT = 3
};


class IScene
{
protected:
	static int sceneNo;

public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();
	int GetSceneNo() { return sceneNo; }
};