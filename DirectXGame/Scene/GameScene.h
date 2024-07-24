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
#include "../Object/Particle/Particle.h"

#include "../GameObject/Skydome/Skydome.h"
#include "../GameObject/Object/Map/Map.h"
#include "../GameObject/Object/Map/Wall/Wall.h"
#include "../GameObject/Object/Map/Item/SmallItem/SmallItem.h"
#include "../GameObject/Entity/Player/Player.h"

class GameScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	KeyInput* input = nullptr;
	Lighting light;

	std::unique_ptr<Camera> camera2;
	Map map;

	Player player;

	SmallItem item;
	SmallItem item2;
};

