#include "GameManager.h"

GameManager* GameManager::GetInstance() {
	static GameManager instance;
	return &instance;
}

GameManager::GameManager() {
	sceneArr[TITLE] = std::make_unique<TitleScene>();
	sceneArr[INGAME] = std::make_unique<GameScene>();
}

GameManager::~GameManager() {

}

void GameManager::Run() {

	CoInitializeEx(0, COINIT_MULTITHREADED);
	//インスタンス

	GlobalVariables::GetInstance()->LoadFiles();

	//GameScene* gameScene = new GameScene;


	sceneArr[INGAME]->Initialize();
	//いろいろ


	//初期化


	//メインループ
	MSG msg{};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			sceneArr[INGAME]->BeginFrame();
			sceneArr[INGAME]->Update();
			sceneArr[INGAME]->Draw();
			sceneArr[INGAME]->EndFrame();
		}
	}
	sceneArr[INGAME]->Final();
	CoUninitialize();

}