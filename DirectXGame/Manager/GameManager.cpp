#include "GameManager.h"

using namespace MyEngine;

GameManager* GameManager::GetInstance() {
	static GameManager instance;
	return &instance;
}

GameManager::GameManager() {

	CoInitializeEx(0, COINIT_MULTITHREADED);
	directX12 = DirectX12::GetInstance();
	windowsAPI = MyEngine::WindowsAPI::GetInstance();
	graphicsRenderer_ = GraphicsRenderer::GetInstance();

	directX12->Initialize();
	graphicsRenderer_->Initialize();

	KeyInput::GetInstance()->Initialize();
	GamePadInput::GetInstance()->Initialize();

	TextureManager::GetInstance()->Initialize();
	ModelManager::GetInstance()->Initialize();
	FileManager::GetInstance()->Initialize();

	sceneArr_[TEST] = std::make_unique<TestScene>();
	sceneArr_[SUBTEST] = std::make_unique<SubTestScene>();
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[INGAME] = std::make_unique<GameScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
}

GameManager::~GameManager() {

}

void GameManager::Initialize() {
	
}

void GameManager::Run() {

	//インスタンス
	GameManager::GetInstance()->Initialize();
	GlobalVariables::GetInstance()->LoadFiles();
	TextureManager::GetInstance()->Initialize();

	//メインループ
	MSG msg{};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			KeyInput::GetInstance()->Update();
			GamePadInput::GetInstance()->BeginFrame();
			//シーンのチェック
			prevSceneNo_ = currentSceneNo_;
			currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

			//シーンが前フレームと異なったら初期化
			if (prevSceneNo_ != currentSceneNo_) {
				sceneArr_[currentSceneNo_]->Initialize();
			}

			BeginFrame();
			sceneArr_[currentSceneNo_]->Update();
			ImGui::Render();
			sceneArr_[currentSceneNo_]->Draw();
			EndFrame();
			sceneArr_[currentSceneNo_]->SceneChange();
		}
	}
	Finalize();
	CoUninitialize();
}

void GameManager::BeginFrame() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	directX12->PreDraw();
	graphicsRenderer_->DecideCommand();
}
void GameManager::EndFrame() {
	directX12->PostDraw();
}
void GameManager::Finalize() {
	directX12->Finalize();
	windowsAPI->Finalize();
	directX12->ResourceLeakCheck();
}