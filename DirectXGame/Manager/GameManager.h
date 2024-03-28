#pragma once

#include <memory>
#include "TextureManager.h"
#include "ModelManager.h"

#include "../Base/GlobalVariables/GlobalVariables.h"
#include "../Base/DirextX12/DirectX12.h"
#include "../Base/WindowsAPI/WindowsAPI.h"
#include "../Base/GraphicsRenderer/GraphicsRenderer.h"

#include "../Scene/IScene.h"
#include "../Scene/TestScene.h"

class DirectX12;
class WindowsAPI;
class GraphicsRenderer;

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
	std::unique_ptr<IScene> sceneArr[5];

	int currentSceneNo;
	int prevSceneNo;

public:
	GameManager();
	~GameManager();

	void Run();

private:
	void Initialize();

	void BeginFrame();
	void EndFrame();
	void Finalize();
};

