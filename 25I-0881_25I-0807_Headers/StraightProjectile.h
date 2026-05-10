#pragma once

#include "Projectile.h"

class StraightProjectile : public Projectile
{
public:
	StraightProjectile();
	StraightProjectile(float startX, float startY, bool facingRight);
	StraightProjectile(float startX, float startY, bool facingRight, bool upward);
	virtual ~StraightProjectile() = 0;

	void update(float deltaTime);
};

class BasicStraightProjectile : public StraightProjectile
{
public:
	BasicStraightProjectile(float startX, float startY, bool facingRight, bool upward);
};
