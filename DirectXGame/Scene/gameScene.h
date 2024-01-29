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
#include "../GameObject/Entity/Enemy/Enemy.h"

#include "IScene.h"

#define TRIANGLECOUNT 2

class DirectX12;
class GraphicsRenderer;
class WindowsAPI;
class Texture;
class Sphere;
class Lighting;
class Input;
class Camera;

class GameScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void isCollsion();
	void Release();

	void BeginFrame() override;
	void EndFrame() override;
	void Final() override;
	void Draw() override;

	void VariableInit();
private:
	TriangleData triangleData[TRIANGLECOUNT];
	SpriteData spriteData;

	DirectX12* directX12_ = nullptr;
	WindowsAPI* windowsAPI_ = nullptr;
	Input* input_ = nullptr;

	GraphicsRenderer* graphicsRenderer_ = new GraphicsRenderer;

	//Sprite* sprite = new Sprite;
	//Sphere* sphere = new Sphere;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;

	std::unique_ptr<Camera> camera;

	DirectionalLight light;

	float colorVolume[3];
	Transform transform;
};