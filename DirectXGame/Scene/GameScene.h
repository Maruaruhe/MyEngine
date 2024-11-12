#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"

#include "Stage/IStage.h"
#include "Stage/ShipStage.h"
#include "Stage/OutsideStage.h"
#include "Stage/InsideStage.h"

//enum StageA {
//	A = 0,
//	B = 1,
//	C = 2
//};

class GameScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneChange() override;

private:
	//Scene
	std::unique_ptr<IStage> stageArr[4];

	//KeyInput* input = nullptr;
	//Lighting light;

	std::unique_ptr<Camera> camera2;

	//Map outsideMap;
	//Map insideMap;

	//int clearCount;

	//Player player;
	//Sprite sprite;

	//Trace trace;

	//bool isS;
	//int sFlame;

	//Sprite s;

	//StageA stage;
	//StageA preStage;

	int currentStageNo;
	int prevStageNo;
};

