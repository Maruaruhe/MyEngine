#pragma once
#include "../Base/Input/Input.h"

enum SCENE {
	NONE = 0,
	TITLE = 1,
	INGAME = 2,
	CLEAR = 3,
	FAULT = 4
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