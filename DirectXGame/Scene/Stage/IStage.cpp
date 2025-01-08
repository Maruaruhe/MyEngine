#include "IStage.h"

int IStage::stageNo = SHIP;

int IStage::flame = 0;
int IStage::time = 0;

bool IStage::toClearScene = false;

Player IStage::player_ = {};

Lighting IStage::light_ = {};

std::unique_ptr<Camera> IStage::camera2;

IStage::~IStage() {}

void IStage::TimeLapse() {
	flame++;
	if (flame % 60 == 0) {
		time++;
	}
	ImGui::Begin("nowTime");
	ImGui::Text("time : %d", time);
	ImGui::End();
}