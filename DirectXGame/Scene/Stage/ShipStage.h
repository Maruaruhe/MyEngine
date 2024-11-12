#pragma once
#include "IStage.h"

class ShipStage : public IStage
{
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~ShipStage();
};