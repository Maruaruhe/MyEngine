#pragma once
#include <Windows.h>
#include <cstdint>
#include "../ImGui/ImGuiWND.h"

#pragma comment(lib,"winmm.lib")
//クライアント領域のサイズ
static const int32_t kCliantWidth = 1280;
static const int32_t kClientHeight = 720;

//WindowsAPI
class WindowsAPI
{
public:
	static WindowsAPI* GetInstance();
	//Initialize
	void Init();

	//Window Setting
	void WindowClass();
	//Window Size
	void WindowSize();
	//Window Create
	void WindowCreate();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//Getter
	HWND GetHwnd() const { return hwnd_; }
	HINSTANCE GetHInstance() { return wc_.hInstance; }

	//Finalize
	void Finalize();

private:
	static WindowsAPI* instance;
	//ウインドウクラスの設定
	WNDCLASS wc_{};

	//ウインドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc_;

	HWND hwnd_ = {};

	WindowsAPI() = default;
	~WindowsAPI() = default;
	WindowsAPI(WindowsAPI&) = delete;
	WindowsAPI& operator=(WindowsAPI&) = delete;
};



