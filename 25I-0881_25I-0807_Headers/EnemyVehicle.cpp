#include "EnemyVehicle.h"


using namespace std;
using namespace sf;

EnemyVehicle::EnemyVehicle()
{
	pistolEquipped = false;
	queuedShot = false;
	attackCooldown = 2;
	attackTimer = 0;
	queuedAttack = false;
	scoreValue = 250;
}

void EnemyVehicle::update(float deltaTime)
{
	if (attackTimer > 0)
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