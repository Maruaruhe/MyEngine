#include "GameManager.h"

GameManager* GameManager::GetInstance() {
	static GameManager instance;
	return &instance;
}

GameManager::GameManager() {
	directX12 = DirectX12::GetInstance();
	windowsAPI = WindowsAPI::GetInstance();

	directX12->Initialize();
	graphicsRenderer_->Initialize();
	graphicsRenderer_->ViewportScissor();

	Input::GetInstance()->Initialize();


	sceneArr[TITLE] = std::make_unique<TitleScene>();
	sceneArr[INGAME] = std::make_unique<GameScene>();
	sceneArr[CLEAR] = std::make_unique<ClearScene>();
}

GameManager::~GameManager() {

}

void GameManager::Initialize() {
	
}

void GameManager::Run() {

	CoInitializeEx(0, COINIT_MULTITHREADED);
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
			Input::GetInstance()->Update();
			//シーンのチェック
			prevSceneNo = currentSceneNo;
			currentSceneNo = sceneArr[currentSceneNo]->GetSceneNo();

			//シーンが前フレームと異なったら初期化
			if (prevSceneNo != currentSceneNo) {
				sceneArr[currentSceneNo]->Initialize();
			}

			BeginFrame();
			GlobalVariables::GetInstance()->Update();
			sceneArr[currentSceneNo]->Update();
			ImGui::Render();
			sceneArr[currentSceneNo]->Draw();
			EndFrame();
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