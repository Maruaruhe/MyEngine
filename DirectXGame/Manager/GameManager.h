#pragma once

#include <memory>
#include "TextureManager.h"
#include "ModelManager.h"
#include "FileManager.h"

#include "../Base/GlobalVariables/GlobalVariables.h"
#include "../Base/DirextX12/DirectX12.h"
#include "../Base/WindowsAPI/WindowsAPI.h"
#include "../Base/GraphicsRenderer/GraphicsRenderer.h"

#include "../Scene/IScene.h"
#include "../Scene/TestScene.h"
#include "../Scene/SubTestScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ClearScene.h"

class DirectX12;
class WindowsAPI;
class GraphicsRenderer;

//GameManagerクラス
class GameManager
{
public:
	static GameManager* GetInstance();
private:
	//
	DirectX12* directX12 = nullptr;
	WindowsAPI* windowsAPI = nullptr;
	GraphicsRenderer* graphicsRenderer_ = nullptr;

	//Scene
	std::unique_ptr<IScene> sceneArr_[6];

	int currentSceneNo_;
	int prevSceneNo_;

public:
	GameManager();
	~GameManager();

	//実行中
	void Run();

private:
	//初期化
	void Initialize();

	//フレームスタート
	void BeginFrame();
	//フレームエンド
	void EndFrame();
	//Finalize
	void Finalize();
};

