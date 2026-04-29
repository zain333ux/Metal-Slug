#pragma once

#include "Projectile.h"

class StraightProjectile : public Projectile
{
public:
	StraightProjectile(float startX, float startY, bool facingRight);

	void update(float deltaTime);
};
