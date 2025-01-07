#pragma once
#include "IStage.h"
#include "../../Manager/FileManager.h"
#include "../../Object/Particle/Particle.h"

#include "../../GameObject/Object/Map/MapJson.h"

//ShipStage
class ShipStage : public IStage
{
public:
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~ShipStage();

private:
	int stageChangeCount;
	Sprite noise0;

	//Model mapModel;
	//Model a;
	LevelData level;

	MapJson json;

	Particle p;

	Trace trace_;
};