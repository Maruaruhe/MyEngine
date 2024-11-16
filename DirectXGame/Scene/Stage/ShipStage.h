#pragma once
#include "IStage.h"

class ShipStage : public IStage
{
public:
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void StageChange() override;

	~ShipStage();

private:
	void CreateModels();
	Model mapModel;
};