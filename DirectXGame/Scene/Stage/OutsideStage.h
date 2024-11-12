#pragma once
#include "IStage.h"

class OutsideStage : public IStage
{
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~OutsideStage();
};