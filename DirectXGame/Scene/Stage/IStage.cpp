#include "IStage.h"
#include "../../Manager/TextureManager.h"

using namespace MyEngine;

int IStage::stageNo = SHIP;

int IStage::flame = 0;
int IStage::time = 0;

bool IStage::toClearScene = false;
bool IStage::toOverScene = false;
bool IStage::isPosed = false;

Sprite IStage::toClear_ = {};
Sprite IStage::isPosed_ = {};

Player IStage::player_ = {};

Lighting IStage::light_ = {};

std::vector<std::shared_ptr<mapItem>> IStage::items_ = {};

std::unique_ptr<Camera> IStage::camera2;

IStage::IStage(){}
IStage::~IStage() {}

bool IStage::ChackIsPosed() {
	if (MyEngine::KeyInput::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		if (!isPosed) {
			isPosed = true;
		}
		else {
			isPosed = false;
		}
	}

	if (isPosed) {
		return true;
	}
	else{
		return false;
	}
}

void IStage::GameInitialize() {
	camera2 = std::make_unique<Camera>();
	camera2->Initialize();

	player_.Initialize();

	light_.Initialize();

	items_ = mapJson_.CreateItem(camera2.get(), "Inside");
	player_.SetItems(items_);

	flame = 0;
	time = 0;

	stageNo = SHIP;

	toClearScene = false;
	toOverScene = false;

	toClear_.Initialize({ 180,35 }, "Resources/Title/toClear.png");
	toClear_.transform.translate = { 640,500,0 };

	isPosed_.Initialize({ 300,40 }, "Resources/Title/isPosed.png");
	isPosed_.transform.translate = { 150,50,0 };
}

void IStage::TimeLapse() {
	flame++;
	if (flame % 60 == 0) {
		time++;
	}
	ImGui::Begin("nowTime");
	ImGui::Text("time : %d", time);
	ImGui::End();
}

void IStage::ClearDraw() {
	if (shipItemNums >= 1) {
		toClear_.Update();
		toClear_.Draw();
	}
	isPosed_.Update();
	isPosed_.Draw();
}

void IStage::ItemUpdate(int currentStage){
	for (std::shared_ptr<mapItem> item : items_) {
		if (item->isTaken_) {
			item->nowWhere_ = nowWhere(currentStage);
		}
		item->Update();
	}
}


void IStage::ItemDraw(int currentStage){
	for (std::shared_ptr<mapItem> item : items_) {
		if (item->nowWhere_ ==  currentStage) {
			item->Draw();
		}
	}
}

void IStage::CheckItemNum(int currentStage) {
	shipItemNums = 0;
	for (std::shared_ptr<mapItem> item : items_) {
		//SHIPの中にあるアイテムの数をカウント
		if (item->nowWhere_ == SH) {
			shipItemNums++;
		}
	}
	if (shipItemNums >= 1) {
		if (MyEngine::KeyInput::GetInstance()->TriggerKey(DIK_C)) {
			toClearScene = true;
		}
	}
	else {
		toClearScene = false;
	}
#ifdef _DEBUG
	ImGui::Begin("items");
	ImGui::SliderInt("num = %d", &shipItemNums, 0, 0);
	ImGui::End();
#endif // _DEBUG
}

void IStage::DeadCheck() {
	if (!player_.GetIsAlive() && player_.deadFlame >= 180) {
		if (MyEngine::KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
			toOverScene = true;
		}
	}
}