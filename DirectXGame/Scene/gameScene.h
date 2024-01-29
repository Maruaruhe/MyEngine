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
	void isCollsion();
	void Release();

	void BeginFrame();
	void EndFrame();
	void Final();
	void Draw();

	void VariableInit();
private:
	TriangleData triangleData[TRIANGLECOUNT];
	SpriteData spriteData;

	DirectX12* directX12_;
	WindowsAPI* windowsAPI_;

	GraphicsRenderer* graphicsRenderer_ = new GraphicsRenderer;
	Input* input_;
	//Triangle** triangle_ = new Triangle * [TRIANGLECOUNT];
	Sprite* sprite = new Sprite;
	////Texture* texture_ = new Texture;
	Sphere* sphere = new Sphere;
	//Model* model = new Model;
	Camera* camera = new Camera;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;

	DirectionalLight light;

	float colorVolume[3];
	Transform transform;

	enum Scene {
		title,
		inGame,
		End
	};
	Scene scene = inGame;
};