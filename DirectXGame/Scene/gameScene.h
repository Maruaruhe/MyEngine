#pragma once
#include "IScene.h"

#include "../Base/Input/Input.h"

#include "../Object/Triangle/Triangle.h"
#include "../Object/Texture/Texture.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"
#include "../Object/Sprite/Sprite.h"
#include "../Object/Model/Model.h"
#include "../Object/Camera/Camera.h"

#include "../GameObject/Entity/Player/Player.h"
#include "../GameObject/Entity/Enemy/Enemy.h"

//class Texture;
//class Sphere;
//class Lighting;
//class Input;
//class Camera;

class GameScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void isCollsion();

	void Draw() override;
private:
	void LoadEnemyPopDate();
	void UpdateEnemyPopCommands();

	void spawnEnemy(Vector3 pos);

	bool checkEnemyAlive();
private:
	Input* input = nullptr;

	std::unique_ptr<Player> player_;
	std::list<Enemy*> enemies_;

	std::unique_ptr<Camera> camera;


	DirectionalLight light;

	float colorVolume[3];

	std::stringstream enemyPopCommands;
};