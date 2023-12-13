#include "gameScene.h"

void GameScene::Initialize(DirectX12* directX12, WindowsAPI* windowsAPI)
{
	//light->Initialize();
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;


	VariableInit();
	directX12_ = directX12;
	directX12_->Init(windowsAPI);
	colorVolume[0] = 1.0f;
	colorVolume[1] = 1.0f;
	colorVolume[2] = 1.0f;

	graphicsRenderer_->Initialize(directX12);

	input_->Initialize(windowsAPI);

	sprite->Initialize(directX12_, spriteData);
	sphere->Initialize(directX12_);
	model->Initialize(directX12_);
	camera->Initialize();

	graphicsRenderer_->ViewportScissor();

	transform.translate = { 0.0f,0.0f,0.0f };
	transform.rotate = { 0.0f,0.0f,0.0f };
	transform.scale = { 1.0f,1.0f,1.0f };

	//texture_->scene(directX12);
}

void GameScene::Update() {
	input_->Update();
	//ImGui::ShowDemoWindow();

	ImGui::ColorEdit3("TriangleColor", colorVolume);
	ImGui::SliderFloat3("TriangleColor", colorVolume, 0.0f, 1.0f);
	camera->Update();
	ImGui::SliderFloat3("lightcolor", &light.color.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("light", &light.direction.x, -1.0f, 1.0f);

	GlobalVariables::GetInstance()->Update();

	Vector4 color = {colorVolume[0],colorVolume[1],colorVolume[2],1.0f};

	sprite->Update(color, camera->GetTransform());
	sphere->Update(color, transform, light);
	model->Update(color, camera->GetTransform(), light);

	ImGui::Render();
}

void GameScene::Release() {
	//directX12_->Release();
	//graphicsRenderer_->Release();

	//sprite->Release();
	//sphere->Release();
	//model->Release();
}



void GameScene::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	directX12_->PreDraw();
	graphicsRenderer_->DecideCommand(directX12_);
}

void GameScene::EndFrame() {
	directX12_->PostDraw();
}

void GameScene::Final() {
	directX12_->ResourceLeakCheck();
}

void GameScene::Draw() {
	sprite->Draw();
	//sphere->Draw();
	model->Draw();
}

void GameScene::VariableInit() {
	spriteData.LeftBot.position = { 0.0f,360.0f,0.0f,1.0f };
	spriteData.LeftBot.texcoord = { 0.0f,1.0f };
	spriteData.LeftTop.position = { 0.0f,0.0f,0.0f,1.0f };
	spriteData.LeftTop.texcoord = { 0.0f,0.0f };
	spriteData.RightBot.position = { 640.0f,360.0f,0.0f,1.0f };
	spriteData.RightBot.texcoord = { 1.0f,1.0f };
	spriteData.RightTop.position = { 640.0f,0.0f,0.0f,1.0f };
	spriteData.RightTop.texcoord = { 1.0f,0.0f };
}
