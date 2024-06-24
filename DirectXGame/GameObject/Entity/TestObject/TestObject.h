#include "../../../Object/Anime/Anime.h"
#include "../../../Base/Input/Input.h"

class TestObject {
public:

	void Initialize();

	void Update();

	void Draw();


	Anime anime;
private:
	GamePadInput* pInput;
	KeyInput* kInput;
};