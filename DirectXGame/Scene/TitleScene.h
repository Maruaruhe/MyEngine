#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"
#include "../Base/GlobalVariables/GlobalVariables.h"

#include "../Object/Sprite/Sprite.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Model/Model.h"
#include "../Object/Anime/Anime.h"

#include "../Math/RandomGenerator.h"

struct tmpParticle {
	Sprite* sprite;
	float velocity;
	float rotate;
};

enum Phase {
	first,
	second,
};

//TitleScene
class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneChange() override;
	
	~TitleScene();

private:
	MyEngine::KeyInput* input = nullptr;
	
	Phase phase_;

	Sprite back;
	Sprite title;
	Sprite explain;


	int sceneChangeFrame;
	bool startTitleChanging;
	bool startExplainChanging;
	bool sceneMove;
};

