#pragma once

#include "Projectile.h"

class RocketProjectile : public Projectile
{
public:
	RocketProjectile(float startX, float startY, bool facingRight, bool airborne);

	void update(float deltaTime);
};
