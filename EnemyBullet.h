#pragma once

#include "Projectile.h"

class EnemyBullet : public Projectile
{
public:
	EnemyBullet(float startX, float startY, bool facingRight);

	void update(float deltaTime);
};
