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

//Scene基底クラス
class IScene
{
protected:
	static int sceneNo;

public:
	/// <summary>
	/// Initialize
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// Update
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// Draw
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// シーン変更
	/// </summary>
	virtual void SceneChange() = 0;

	virtual ~IScene();
	int GetSceneNo() { return sceneNo; }
};