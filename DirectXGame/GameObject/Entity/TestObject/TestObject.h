#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Base/Input/Input.h"

class TestObject {
public:

	void Initialize();

	void Update();

	void Draw();


	Anime anime;
	Model model;
private:
	GamePadInput* pInput;
	KeyInput* kInput;
};