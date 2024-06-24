#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

KeyInput* KeyInput::GetInstance() {
	static KeyInput instance;
	return &instance;
}

void KeyInput::Initialize() {
	winAPI = WindowsAPI::GetInstance();

	HRESULT result;
	//DiretxInputのインスタンス生成
	//ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(winAPI->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイス作成
	//ComPtr<IDirectInputDevice8> keyboard;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//入力デーら形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//排他制御レベルのリセット
	result = keyboard->SetCooperativeLevel(winAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void KeyInput::Update() {
	//HRESULT result;
	//前回のキー入力を保存
	memcpy(preKey, key, sizeof(key));
	//キーボード情報の取得開始
	keyboard->Acquire();
	//全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);
}

bool KeyInput::PushKey(BYTE keyNumber) {
	if (key[keyNumber]) {
		return true;
	}
	return false;
}

bool KeyInput::TriggerKey(BYTE keyNumber) {
	if (key[keyNumber] && !preKey[keyNumber]) {
		return true;
	}
	return false;
}


// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------

// インスタンスの取得
GamePadInput* GamePadInput::GetInstance() {
	static GamePadInput instance;
	return &instance;
}

// 初期化処理
void GamePadInput::Initialize() {

	// 各ボタンのトリガー状態の初期化処理
	GamePadInput::GetInstance()->ResetButtonTriggers();

}

// 更新処理
void GamePadInput::BeginFrame() {

	// メモリコピー
	GamePadInput::GetInstance()->preJoyState_ = GamePadInput::GetInstance()->joyState_;

	GamePadInput::GetInstance()->GetJoyState();

	// ジョイスティックの状態をポーリング
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
	}
}

// パッドの状態更新
bool GamePadInput::GetJoyState()
{
	DWORD dwResult = XInputGetState(0, &GamePadInput::GetInstance()->joyState_);
	if (dwResult == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

// 各ボタンのトリガー状態の初期化処理
void GamePadInput::ResetButtonTriggers()
{
	ZeroMemory(buttonTriggers, sizeof(buttonTriggers));
}

// ジョイコンの入力の取得
bool GamePadInput::GetJoyStickState()
{
	DWORD dwresult = XInputGetState(0, &GamePadInput::GetInstance()->joyState_);

	if (dwresult == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

// 押されていない
bool GamePadInput::NoneButton(PadData button)
{
	button;
	return false;
}

// 押した瞬間
bool GamePadInput::TriggerButton(PadData button)
{
	bool preFlag = false;

	if (GamePadInput::GetInstance()->preJoyState_.Gamepad.wButtons & (WORD)button)
	{
		preFlag = true;
	}

	if (!preFlag && GamePadInput::GetInstance()->joyState_.Gamepad.wButtons & (WORD)button)
	{
		return true;
	}

	return false;
}

// 押しっぱなし
bool GamePadInput::PressButton(PadData button)
{
	if (GamePadInput::GetInstance()->joyState_.Gamepad.wButtons & (WORD)button)
	{
		return true;
	}
	return false;
}

// 離された瞬間
bool GamePadInput::ReleaseButton(PadData button)
{
	button;
	return false;
}

// Lスティック
Vector2 GamePadInput::GetLStick(const float& mode)
{
	return {
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbLX / mode,
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbLY / mode };
}

// Rスティック
Vector2 GamePadInput::GetRStick(const float& mode)
{
	return {
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbRX / mode,
		GamePadInput::GetInstance()->joyState_.Gamepad.sThumbRY / mode };
}

// -------------------------------------------------------------------------
// GamePadInput : ゲームパッド
// -------------------------------------------------------------------------