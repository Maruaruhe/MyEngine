#pragma once
#include "IStage.h"

class OutsideStage : public IStage
{
public:
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~OutsideStage();
};