#pragma once

#include "Projectile.h"

class MeleeHitbox : public Projectile
{
public:
	MeleeHitbox(float startX, float startY, bool facingRight, bool airborne);
};
