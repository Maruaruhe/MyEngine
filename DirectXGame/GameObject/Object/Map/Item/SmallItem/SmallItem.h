#pragma once
#include "../IItem.h"
#include "../../../../../Object/AABB/AABB.h"

class SmallItem : public IItem
{
public:
	void Initialize(Transform transform, Camera* camera);
	void Update();
	void Draw();

	void CheckLineCollision(Segment segment);
private:

};

