#pragma once

#include "Projectile.h"

class StraightProjectile : public Projectile
{
public:
	StraightProjectile(float startX, float startY, bool facingRight);
	StraightProjectile(float startX, float startY, bool facingRight, bool upward);

	void update(float deltaTime);
};
