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
#include "../../GameObject/Object/Map/Map.h"
#include "../../GameObject/Object/Map/Wall/Wall.h"
#include "../../GameObject/Object/Map/Item/SmallItem/SmallItem.h"
#include "../../GameObject/Entity/Player/Player.h"
				  
#include "../../GameObject/Entity/Enemy/Trace/Trace.h"


enum Stage {
	SHIP = 1,
	OUTSIDE = 2,
	INSIDE = 3
};

class IStage
{
protected:
	static int stageNo;

	static Player player_;

	static Lighting light_;

	static std::unique_ptr<Camera> camera2;

	//static 
	Map map_;

public:
	virtual void Initialize(int prevStage) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void StageChange() = 0;

	virtual ~IStage();
	int GetStageNo() { return stageNo; }

private :
};

