#include "EnemyState.h"

void EnemyStateApproach::Update(Enemy* enemy) {
	const Vector3 velocity = { 0.0f, 0.0f, -0.1f };
	enemy->Move(velocity);
	if (enemy->GetTransform().translate.z < 10.0f) {
		enemy->ChangeState(new EnemyStateLeave());
	}
}
void EnemyStateLeave::Update(Enemy* enemy) {
	const Vector3 velocity = { 0.0f, 0.0f, 0.1f };
	enemy->Move(velocity);
	if (enemy->GetTransform().translate.z > 35.0f) {
		enemy->ChangeState(new EnemyStateApproach());
	}
}