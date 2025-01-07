#include "DirectXGame/Manager/GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GameManager::GetInstance()->Run();
	return 0;
}