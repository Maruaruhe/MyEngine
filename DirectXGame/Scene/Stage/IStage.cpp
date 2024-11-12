#include "IStage.h"

int IStage::stageNo = SHIP;

Player IStage::player_ = {};

Lighting IStage::light_ = {};

std::unique_ptr<Camera> IStage::camera2;

IStage::~IStage() {}