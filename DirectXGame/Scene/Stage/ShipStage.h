#pragma once
#include "IStage.h"
#include "../../Manager/FileManager.h"
#include "../../Object/Particle/Particle.h"

#include "../../GameObject/Object/Map/MapJson.h"

//ShipStage
class ShipStage : public IStage
{
public:
	void FirstInitialize(Player* player) override;
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void StageChange() override;

	~ShipStage();

private:
	int stageChangeCount;

	LevelData level;

	Particle p;
};