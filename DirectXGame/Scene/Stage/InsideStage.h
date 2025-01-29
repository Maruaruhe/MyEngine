#pragma once
#include "IStage.h"

//InsideStage
class InsideStage : public IStage
{
public:
	void FirstInitialize(Player* player) override;
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void StageChange() override;

	~InsideStage();

private:
	int stageChangeCount;

	Trace trace;
};