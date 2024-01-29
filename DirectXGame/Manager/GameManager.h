#pragma once

#include <memory>

#include "../Scene/IScene.h"
#include	"../Scene/GameScene.h"
#include	"../Scene/TitleScene.h"

class GameManager
{
public:
	static GameManager* GetInstance();
private:
	std::unique_ptr<IScene> sceneArr[4];

	int currentSceneNo;
	int prevSceneNo;

public:
	GameManager();
	~GameManager();

	void Run();
};

