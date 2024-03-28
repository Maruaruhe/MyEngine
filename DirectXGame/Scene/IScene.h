#pragma once
#include "../Base/Input/Input.h"

enum SCENE {
	NONE = 0,
	TEST = 1,
	SUBTEST = 2,
	TITLE = 3,
	INGAME = 4,
	CLEAR = 5
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