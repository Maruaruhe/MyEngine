#include "TitleScene.h"
#include "../Manager/ModelManager.h"

using namespace MyEngine;

TitleScene::~TitleScene() {
	for (tmpParticle* s : blacks) {
		delete s;
	}
}

void TitleScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Title/title.png");
	title.Initialize({1280,720}, "Resources/Title/title.png");
	title.transform.translate = { 640,360 ,0};
	title.anchorPoint = { 0.5f,0.5f };

	TextureManager::GetInstance()->LoadTexture("Resources/Title/titleHeart.png");

	transXScope.min = -100.0f;
	transXScope.max = 1280.0f;

	timeScope.min = 30.0f;
	timeScope.max = 60.0f;

	sScope.min = 0.3f;
	sScope.max = 1.0f;

	vScope.min = 5.3f;
	vScope.max = 7.3f;

	setTime = 15;

	sceneChangeFrame = 0;
	startChanging = false;
	howManyParticle = 1;
}

void TitleScene::Update() {
	bUpdate();
	blacks.remove_if([](tmpParticle* black) {
		if (black->sprite->transform.translate.y <= -black->sprite->size_.y) {
			delete black;
			return true;
		}
		return false;
		});
}

void TitleScene::Draw() {
	title.Draw();
	for (tmpParticle* b : blacks) {
		b->sprite->transform.translate.y -= b->velocity;
		b->sprite->Draw();
	}
}

void TitleScene::SceneChange() {
	if (sceneChangeFrame >= 600) {
		sceneNo = INGAME;

		//for (tmpParticle* s : blacks) {
		//	delete s;
		//}
	}
}

void TitleScene::bInit(){

}

void TitleScene::bUpdate(){
	ct--;

	if (ct <= 0) {
		for (int i = 0; i < howManyParticle; i++) {
			CreateParticle();
		}
	}
	//シーン遷移スタート
	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		startChanging = true;
	}

	//フレーム計算
	if (startChanging) {
		sceneChangeFrame++;
		
		if (sceneChangeFrame >= 0) {
			howManyParticle = 3;
			setTime = 3;
		}

		if (sceneChangeFrame >= 120) {
			howManyParticle = 8;
			setTime = 2;
		}
		if (sceneChangeFrame >= 360) {
			howManyParticle = 3;
			setTime = 4;
		}
		if (sceneChangeFrame >= 480) {
			howManyParticle = 1;
			setTime = 30;
		}


	}

}

void TitleScene::CreateParticle() {
	tmpParticle* b = new tmpParticle();
	b->sprite = new Sprite();

	float x = RandomGenerator::GetInstance()->getRandom(transXScope);
	float s = RandomGenerator::GetInstance()->getRandom(sScope);

	float v = RandomGenerator::GetInstance()->getRandom(vScope);

	//b->sprite->Initialize({ x - 100,y }, { x, y + 100 }, "Resources/Title/aaa.png");
	b->sprite->Initialize({ 100, 100 }, "Resources/Title/titleHeart.png");

	b->sprite->transform.translate.x = x;
	b->sprite->transform.translate.y = 720;

	b->velocity = v;
	b->sprite->transform.scale *= s;
	blacks.push_back(b);

	ct = int(setTime);
	//ct = int(0);
}