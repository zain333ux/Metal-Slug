#pragma once

#include "Projectile.h"

class GrenadeProjectile : public Projectile
{
public:
	GrenadeProjectile(float startX, float startY, bool facingRight, bool airborne);

	void update(float deltaTime);
};
