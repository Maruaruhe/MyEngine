#pragma once
#include "../IItem.h"
#include "../../../../../Object/AABB/AABB.h"

class SmallItem : public IItem
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="camera"></param>
	void Initialize(Transform transform, Camera* camera);
	/// <summary>
	/// Update
	/// </summary>
	void Update();
	/// <summary>
	/// Draw
	/// </summary>
	void Draw();

	/// <summary>
	/// ライン判定
	/// </summary>
	/// <param name="segment"></param>
	void CheckLineCollision(Segment segment);
private:

};

