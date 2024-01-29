#include "DirectXGame/Manager/GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager game;
	game.Run();
	//GameManager::GetInstance()->Run();
	return 0;
}