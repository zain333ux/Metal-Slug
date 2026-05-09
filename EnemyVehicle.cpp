#include "EnemyVehicle.h"

EnemyVehicle::EnemyVehicle()
{
	pistolEquipped = false;
	queuedShot = false;
	attackCooldown = 2.0f;
	attackTimer = 0.0f;
	queuedAttack = false;
	scoreValue = 250;
}

void EnemyVehicle::update(float deltaTime)
{
	if (attackTimer > 0.0f)
	{
		attackTimer -= deltaTime;
	}

	Enemy::update(deltaTime);
}

Projectile* EnemyVehicle::createProjectileIfReady()
{
	if (!queuedAttack)
	{
		return 0;
	}

	queuedAttack = false;
	return attack();
}
