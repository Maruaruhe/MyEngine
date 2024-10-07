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

class TitleScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneChange() override;

	void bInit();
	void bUpdate();

private:
	KeyInput* input = nullptr;
	
	Sprite title;
	Sprite black;

	std::vector<Sprite> blacks;
	int ct;

	Model model;

	Scope tScope;
	Scope lScope;
	Scope yScope;
	Scope rScope;
	Scope sScope;

	float velocity = 0.3f;

	std::unique_ptr<Camera> camera2;
};

