#pragma once
#include "IStage.h"

//OutsideStage
class OutsideStage : public IStage
{
public:
	void FirstInitialize(Player* player) override;
	void Initialize(int prevStage) override;
	void Update() override;
	void Draw() override;
	void SpriteDraw() override;
	void StageChange() override;

	void SetItem(std::vector<mapItem*> items) { items_ = items; }

	~OutsideStage();

private:
	int stageChangeCount;
	std::vector<mapItem*> items_;
};