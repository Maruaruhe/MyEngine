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
	Vector3 distance{};

	int tracingTime;

	float maxSpeed = 0.20f;
	float firstSpeed = 0.05f;
	float chaseSpeed;
};

