#pragma once
#include "IStage.h"

class InsideStage : public IStage
{
public:
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~InsideStage();
};