#include "gameScene.h"

void GameScene::Initialize()
{
	//light->Initialize();
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;


	VariableInit();
	directX12_ = DirectX12::GetInstance();
	windowsAPI_ = WindowsAPI::GetInstance();
	directX12_->Init(windowsAPI_);
	colorVolume[0] = 1.0f;
	colorVolume[1] = 1.0f;
	colorVolume[2] = 1.0f;

	graphicsRenderer_->Initialize(directX12_);

	Input::GetInstance()->Initialize(windowsAPI_);

	sprite->Initialize(directX12_, spriteData);
	sphere->Initialize(directX12_);

	model = std::make_unique<Model>();
	model->Initialize(directX12_, "ghostPori");
	camera->Initialize();

	graphicsRenderer_->ViewportScissor();

	transform.translate = { 0.0f,0.0f,0.0f };
	transform.rotate = { 0.0f,0.0f,0.0f };
	transform.scale = { 1.0f,1.0f,1.0f };

	//texture_->scene(directX12);
}

void GameScene::Update() {
	Input::GetInstance()->Update();
	//ImGui::ShowDemoWindow();

	ImGui::Begin("s");
	ImGui::ColorEdit3("TriangleColor", colorVolume);
	ImGui::SliderFloat3("TriangleColor", colorVolume, 0.0f, 1.0f);
	camera->Update();
	ImGui::SliderFloat3("lightcolor", &light.color.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("light", &light.direction.x, -1.0f, 1.0f);
	ImGui::End();


	GlobalVariables::GetInstance()->Update();

	Vector4 color = {colorVolume[0],colorVolume[1],colorVolume[2],1.0f};

	sprite->Update(color,transform);
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
