#pragma once
#include "../../../../Object/Anime/Anime.h"
#include "../../../../Object/Model/Model.h"
#include "../../../../Base/Input/Input.h"

#include "../../../../Manager/FileManager.h"

//Wallクラス
class Door
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="scale"></param>
	void Initialize(const Transform transform, std::string filename);

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