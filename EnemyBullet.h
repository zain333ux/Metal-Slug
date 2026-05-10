#pragma once

#include "StraightProjectile.h"

class EnemyBullet : public StraightProjectile
{
public:
	EnemyBullet(float startX, float startY, bool facingRight);

	void update(float deltaTime);
};
