#pragma once

#include "Projectile.h"

class EnemyRocketProjectile : public Projectile
{
private:
	bool exploded;
	float explosionTimer;

	void explode();

public:
	EnemyRocketProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY);

	void update(float deltaTime);
};
