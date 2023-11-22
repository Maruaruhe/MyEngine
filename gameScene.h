#pragma once
#include "DirectX12.h"
#include "GraphicsRenderer.h"
#include "WindowsAPI.h"
#include "Triangle.h"
#include "ImGuiWND.h"
#include "Texture.h"
#include "Sphere.h"
#include "Lighting.h"
#include "Sprite.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"

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
	void Initialize(DirectX12* directX12, WindowsAPI* windowsAPI);
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

	DirectX12* directX12_ = new DirectX12;
	GraphicsRenderer* graphicsRenderer_ = new GraphicsRenderer;
	Input* input_ = new Input;
	Triangle** triangle_ = new Triangle * [TRIANGLECOUNT];
	Sprite* sprite = new Sprite;
	//Texture* texture_ = new Texture;
	Sphere* sphere = new Sphere;
	Model* model = new Model;
	Camera* camera = new Camera;

	DirectionalLight light;

	float colorVolume[3];
	Transform transform;
};

