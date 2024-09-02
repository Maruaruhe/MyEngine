#pragma once
#include "../IEnemy.h"

class Wall;
class Trace : public IEnemy
{
public:
	void Initialize(Transform transform, Camera* camera, Map* map, Player* player);
	void Update();
	void Draw();
private:
	void FindPlayer();
	void ChasePlayer();

	std::vector<Wall> walls;
	Vector3 velocity{};
	int tracingTime;
};

