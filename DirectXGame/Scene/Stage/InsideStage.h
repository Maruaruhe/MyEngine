#pragma once
#include "IStage.h"

//InsideStage
class InsideStage : public IStage
{
public:
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~InsideStage();
};