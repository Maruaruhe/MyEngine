#include "WindowsAPI.h"

WindowsAPI* WindowsAPI::instance = nullptr;

WindowsAPI* WindowsAPI::GetInstance() {
	//static WindowsAPI instance;
	//return &instance;
	if (instance == nullptr) {
		instance = new WindowsAPI;
	}
	return instance;
}

void WindowsAPI::Init() {
	timeBeginPeriod(1);
	WindowClass();
	WindowSize();
	WindowCreate();
}




LRESULT CALLBACK WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WindowsAPI::WindowClass() {


	//ウインドウプロシージャ	
	wc_.lpfnWndProc = WindowProc;
	//ウインドウクラス名（なんでもいい）
	wc_.lpszClassName = L"廃品回収";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウインドウクラスを登録する
	RegisterClass(&wc_);
}

void WindowsAPI::WindowSize() {
	wrc_ = { 0,0,kCliantWidth,kClientHeight };
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);
}

void WindowsAPI::WindowCreate() {
	//ウインドウの生成
	hwnd_ = CreateWindow(
		wc.lpszClassName,		//利用するクラス名
		L"廃品回収",			//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//よく見るウインドウスタイル
		CW_USEDEFAULT,			//表示X座標（Windowsに任せる）
		CW_USEDEFAULT,			//表示Y座標（Windowsに任せる）
		wrc.right - wrc.left,	//ウインドウ横幅
		wrc.bottom - wrc.top,	//ウインドウ縦幅
		nullptr,				//ウインドウハンドル
		nullptr,				//メニューハンドル
		wc.hInstance,			//インスタンスハンドル
		nullptr
	);

	/*directX12->DebugLayer();*/

	ShowWindow(hwnd_, SW_SHOW);
}



void WindowsAPI::Finalize() {
	delete instance;
	instance = nullptr;
}