#pragma once
#include "IStage.h"

//OutsideStage
class OutsideStage : public IStage
{
public:
	void FirstInitialize() override;
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void StageChange() override;

	~OutsideStage();

private:
	int stageChangeCount;
};