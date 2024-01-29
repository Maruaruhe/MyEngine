#include "gameScene.h"

void GameScene::Initialize()
{
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;


	VariableInit();
	directX12_ = DirectX12::GetInstance();
	windowsAPI_ = WindowsAPI::GetInstance();
	directX12_->Initialize();
	colorVolume[0] = 1.0f;
	colorVolume[1] = 1.0f;
	colorVolume[2] = 1.0f;

	graphicsRenderer_->Initialize();

	Input::GetInstance()->Initialize();

	camera->Initialize();

	player_ = std::make_unique<Player>();
	enemy_ = std::make_unique<Enemy>();

	player_->Initialize("ghostPori");
	enemy_->Initialize("ghostPori");

	sphere->Initialize();

	graphicsRenderer_->ViewportScissor();

	transform.translate = { 0.0f,0.0f,0.0f };
	transform.rotate = { 0.0f,0.0f,0.0f };
	transform.scale = { 1.0f,1.0f,1.0f };
}

void GameScene::Update() {
	Input::GetInstance()->Update();
	GlobalVariables::GetInstance()->Update();

	Vector4 color = {colorVolume[0],colorVolume[1],colorVolume[2],1.0f};

	player_->Update(color, camera->GetTransform(), light);
	Vector4 eColor = { 0.0f,0.0f,0.0f,1.0f };
	enemy_->Update(eColor, camera->GetTransform(), light);
	Vector3 a = { 1,1,1 };
	sphere->Update(color, { 1,1,1 }, camera->GetTransform(), light);
	isCollsion();
	ImGui::Render();
}

void GameScene::isCollsion() {
	Vector3 pPos = player_->GetBulletTransform().translate;
	Vector3 ePos = enemy_->GetTransform().translate;
	Vector3 dis = Subtract(pPos, ePos);
	if ((dis.x * dis.x + dis.y * dis.y + dis.z * dis.z) <= 2) {
		enemy_->SetIsAlive(false);
	}
}

void GameScene::Release() {

}



void GameScene::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	directX12_->PreDraw();
	graphicsRenderer_->DecideCommand();
}

void GameScene::EndFrame() {
	directX12_->PostDraw();
}

void GameScene::Final() {
	directX12_->ResourceLeakCheck();
}

void GameScene::Draw() {
	player_->Draw();
	enemy_->Draw();

	sphere->Draw();
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
