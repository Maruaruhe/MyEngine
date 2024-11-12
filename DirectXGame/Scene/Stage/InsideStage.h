#pragma once
#include "IStage.h"

class InsideStage : public IStage
{
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~InsideStage();
};