#pragma once
#include "../../../../Object/Anime/Anime.h"
#include "../../../../Object/Model/Model.h"
#include "../../../../Base/Input/Input.h"

class Wall
{
public:
	void Initialize(const Vector3 pos, const Vector3 scale);

	void Update();

	void Draw();

	Model model;
private:

};