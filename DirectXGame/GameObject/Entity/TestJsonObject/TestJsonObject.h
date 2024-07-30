#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Base/Input/Input.h"

class TestJsonObject {
public:

	void Initialize();

	void Update();

	void Draw();

	std::unique_ptr<Model> model;
	std::unique_ptr<Model> model2;
	std::unique_ptr<Model> model3;
private:
	GamePadInput* pInput;
	KeyInput* kInput;
};