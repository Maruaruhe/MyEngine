#pragma once
#include "../../../Object/Model/Model.h"
#include "EnemyBullet/EnemyBuullet.h"
#include "../TimedCall/TimedCall.h"
#include <list>

class GameScene;

class Player;

class Enemy;

class BaseEnemyState {
public:
	virtual void Update(Enemy* enemy) = 0;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Update(Enemy* enemy);
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* enemy);
};

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(const std::string& filename, Player* player, const Vector3& pos);
	void Update();
	void Draw();

	void ChangeState(BaseEnemyState* newState);
	void move(const Vector3& velocity);

	void Fire();
	Vector3 SetPosition(const Vector3& position) { return position; }
	Vector3 SetVelocity(const Vector3& velocity) { return velocity; }

	void ResetTime();

	Vector3 GetWorldPosition() { return model->transform.translate; }

	void SetPlayer(Player* player) { player = player; }


	void OnCollision();

	//const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() { return isDead; }

	const std::list<EnemyBullet*>& GetBullets() { return bullets; }

	std::unique_ptr<Model> model;
private:
	GameScene* gameScene_ = nullptr;

	static const int kFireInterval = 120;
	int32_t bTimer = 0;

	Vector3 velocity;

	BaseEnemyState* state;

	Player* player = nullptr;

	std::list<EnemyBullet*> bullets;
	std::list<TimedCall*> timedCalls;

	bool isDead;
};