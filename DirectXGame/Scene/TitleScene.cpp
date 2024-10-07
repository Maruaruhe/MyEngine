#include "TitleScene.h"
#include "../Manager/ModelManager.h"

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
}

void TitleScene::Update() {
	bUpdate();
}

void TitleScene::Draw() {
	title.Draw();
	for (Sprite b : blacks) {
		b.transform.translate.y -= velocity;

		b.Draw();
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
		Sprite b;

		float x = RandomGenerator::GetInstance()->getRandom(tScope);
		float y = RandomGenerator::GetInstance()->getRandom(yScope);
		float s = RandomGenerator::GetInstance()->getRandom(sScope);

		float r = RandomGenerator::GetInstance()->getRandom(lScope);

		if (x >= 200 && x <= 1080 && y >= 200 && y<= 400) {
			ct = 0;
		}
		else {
			b.Initialize({ x - 100,y }, { x, y + 100 }, "Resources/Title/aaa.png");
			//b.transform.scale *= s;
			blacks.push_back(b);

			ct = int(r);
		}
	}

}