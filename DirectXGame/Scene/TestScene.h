#pragma once
#include "IScene.h"
#include "../Base/Input/Input.h"
#include "../Base/GlobalVariables/GlobalVariables.h"

#include "../Object/Sprite/Sprite.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"
#include "../Object/Camera/Camera.h"
#include "../Object/Model/Model.h"
#include "../Object/Particle/Particle.h"

#include "../GameObject/Entity/Player/Player.h"
#include "../GameObject/Entity/Enemy/Enemy.h"
#include "../GameObject/Skydome/Skydome.h"

class TestScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void CheckAllCollision();
private:
	Input* input = nullptr;
	Lighting light;

	Camera* camera2;

	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	std::unique_ptr<Skydome> skydome;
	std::unique_ptr<Sprite> sprite;
};

