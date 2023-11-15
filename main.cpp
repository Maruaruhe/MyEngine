#include "gameScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	CoInitializeEx(0, COINIT_MULTITHREADED);
	//インスタンス
	GameScene* gameScene = new GameScene;

	WindowsAPI* windowsAPI = new WindowsAPI;
	DirectX12* directX12 = new DirectX12;


	gameScene->Initialize(directX12,windowsAPI);
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
			gameScene->BeginFrame();
			gameScene->Update();
			gameScene->Draw();
			gameScene->EndFrame();
		}
	}
	gameScene->Final();
	CoUninitialize();
	return 0;
}