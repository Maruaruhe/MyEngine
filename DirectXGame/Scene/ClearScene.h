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

//ClearScene
class ClearScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneChange() override;

private:
	MyEngine::KeyInput* input = nullptr;

	Sprite title;
	Sprite black;

	Model model;

	std::unique_ptr<Camera> camera2;
};

