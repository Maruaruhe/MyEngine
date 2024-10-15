#include "TitleScene.h"
#include "../Manager/ModelManager.h"

TitleScene::~TitleScene() {
	for (tmpParticle* enemy : blacks) {
		delete enemy;
	}
}

void TitleScene::Initialize() {
	input = KeyInput::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Title/title.png");
	title.Initialize({0,0}, {1280,720}, "Resources/Title/title.png");

	TextureManager::GetInstance()->LoadTexture("Resources/Title/aaa.png");

	tScope.min = 0.0f;
	tScope.max = 1280.0f;
	
	yScope.min = 0.0f;
	yScope.max = 620.0f;

	lScope.min = 30.0f;
	lScope.max = 60.0f;

	sScope.min = 0.3f;
	sScope.max = 1.0f;

	vScope.min = 0.3f;
	vScope.max = 1.3f;

	rScope.min = -0.3f;
	rScope.max = -0.3f;
}

void TitleScene::Update() {
	bUpdate();
}

void TitleScene::Draw() {
	title.Draw();
	for (tmpParticle* b : blacks) {
		b->sprite->transform.translate.y -= b->velocity;
		b->sprite->transform.rotate.z -= b->rotate;
		//b->sprite->transform.scale *= 0.97f;

		b->sprite->Draw();
	}
}

void TitleScene::SceneChange() {
	if (KeyInput::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = INGAME;
	}
}

void TitleScene::bInit(){

}

void TitleScene::bUpdate(){
	ct--;

	if (ct <= 0) {
		tmpParticle* b = new tmpParticle();
		b->sprite = new Sprite();

		float x = RandomGenerator::GetInstance()->getRandom(tScope);
		float y = RandomGenerator::GetInstance()->getRandom(yScope);
		float s = RandomGenerator::GetInstance()->getRandom(sScope);

		float ro = RandomGenerator::GetInstance()->getRandom(rScope);

		float r = RandomGenerator::GetInstance()->getRandom(lScope);

		float v = RandomGenerator::GetInstance()->getRandom(vScope);

		if (x >= 200 && x <= 1080 && y >= 200 && y<= 400) {
			ct = 0;
		}
		else {
			//b->sprite->Initialize({ x - 100,y }, { x, y + 100 }, "Resources/Title/aaa.png");
			b->sprite->Initialize({ 0, 0 }, { 100, 100 }, "Resources/Title/aaa.png");
			//b->sprite->anchorPoint = { 0.5f,0.5f };

			b->sprite->transform.translate.x = x;
			b->sprite->transform.translate.y = y;

			b->velocity = v;
			b->rotate = ro;
			b->sprite->transform.scale *= s;
			blacks.push_back(b);

			ct = int(r);
			ct = int(180);
		}
	}

}