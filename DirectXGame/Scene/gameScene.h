#pragma once
#include "../Base/DirextX12/DirectX12.h"
#include "../Base/GraphicsRenderer/GraphicsRenderer.h"
#include "../Base/WindowsAPI/WindowsAPI.h"
#include "../Object/Triangle/Triangle.h"
#include "../Base/ImGui/ImGuiWND.h"
#include "../Object/Texture/Texture.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Light/Lighting.h"
#include "../Object/Sprite/Sprite.h"
#include "../Object/Model/Model.h"
#include "../Base/Input/Input.h"
#include "../Object/Camera/Camera.h"
#include "../Base/GlobalVariables/GlobalVariables.h"

#include "../GameObject/Entity/Player/Player.h"

#include <iostream>
#include <memory>

#define TRIANGLECOUNT 2

class DirectX12;
class GraphicsRenderer;
class WindowsAPI;
class Texture;
class Sphere;
class Lighting;
class Input;
class Camera;

class GameScene
{
public:
	void Initialize();
	void Update();
	void Release();

	void BeginFrame();
	void EndFrame();
	void Final();
	void Draw();

	void VariableInit();
private:
	TriangleData triangleData[TRIANGLECOUNT];
	SpriteData spriteData;

	DirectX12* directX12_ = nullptr;
	WindowsAPI* windowsAPI_ = nullptr;
	Camera* camera_ = nullptr;

	GraphicsRenderer* graphicsRenderer_ = new GraphicsRenderer;
	Input* input_ = new Input;
	//Triangle** triangle_ = new Triangle * [TRIANGLECOUNT];
	//Sprite* sprite = new Sprite;
	////Texture* texture_ = new Texture;
	//Sphere* sphere = new Sphere;

	//std::unique_ptr<Model> model;
	//std::unique_ptr<Player> player_;
	Player* player_;

	DirectionalLight light;

	float colorVolume[3];
	Transform transform;
};
