#include "../../../Object/Anime/Anime.h"
#include "../../../Object/Model/Model.h"
#include "../../../Base/Input/Input.h"

class TestObject {
public:

	/// <summary>
	/// Initialize
	/// </summary>
	void Initialize();

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Draw
	/// </summary>
	void Draw();


	Anime anime;
	Anime anime2;
	Anime anime3;

	Model model;
private:
	MyEngine::GamePadInput* pInput;
	MyEngine::KeyInput* kInput;
};