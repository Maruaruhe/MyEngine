#pragma once
#include "../../Base/Input/Input.h"

#include "../../Base/GlobalVariables/GlobalVariables.h"

#include "../../Object/Sprite/Sprite.h"
#include "../../Object/Sphere/Sphere.h"
#include "../../Object/Light/Lighting.h"
#include "../../Object/SpotLight/SpotLight.h"
#include "../../Object/Camera/Camera.h"
#include "../../Object/Model/Model.h"
#include "../../Object/Anime/Anime.h"

#include "../../GameObject/Skydome/Skydome.h"
#include "../../GameObject/Object/Map/MapJson.h"
#include "../../GameObject/Object/Map/Wall/Wall.h"
#include "../../GameObject/Object/Map/Item/SmallItem/SmallItem.h"
#include "../../GameObject/Entity/Player/Player.h"
				  
#include "../../GameObject/Entity/Enemy/Trace/Trace.h"


enum Stage {
	SHIP = 1,
	OUTSIDE = 2,
	INSIDE = 3
};

//Stage基底クラス
class IStage
{
protected:
	//現在のステージ
	static int stageNo;

	static bool toClearScene;

	//現在の時刻
	static int flame;
	static int time;

	//初回ロードかどうか
	bool isFirstLoad = true;

	static Player player_;

	static Lighting light_;

	static std::unique_ptr<Camera> camera2;

	MapJson mapJson_;

	static Sprite toggleLight_;

public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="prevStage"></param>
	virtual void Initialize(int prevStage) = 0;
	/// <summary>
/// FirstInitialize
/// </summary>
	virtual void FirstInitialize() = 0;
	/// <summary>
	/// Update
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// Draw
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
/// Draw
/// </summary>
	virtual void SpriteDraw() = 0;
	/// <summary>
	/// Stage変更
	/// </summary>
	virtual void StageChange() = 0;

	void GameInitialize();

	void TimeLapse();
	void SpriteView();

	virtual ~IStage();

	//Getter
	int GetStageNo() { return stageNo; }
	bool GetToClear() {return toClearScene;}

private :
};

