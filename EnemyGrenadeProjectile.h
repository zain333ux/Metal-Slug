#pragma once

#include "Projectile.h"

class EnemyGrenadeProjectile : public Projectile
{
private:
	bool exploded;
	float explosionTimer;

	void explode();

public:
	EnemyGrenadeProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY);

	void update(float deltaTime);
};
