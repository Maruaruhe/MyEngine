#pragma once
#include "../../../../Object/Anime/Anime.h"
#include "../../../../Object/Model/Model.h"
#include "../../../../Base/Input/Input.h"

class Wall
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="scale"></param>
	void Initialize(const Vector3 pos, const Vector3 scale);

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Draw
	/// </summary>
	void Draw();

	Model model_;
private:

};