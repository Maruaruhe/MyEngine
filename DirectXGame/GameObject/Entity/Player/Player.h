#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Base/Input/Input.h"

class Player {
public:

	void Initialize();

	void Update();

	void Draw();

	Model model;

private:
	void Move();

private:
	GamePadInput* pInput;
	KeyInput* kInput;
};