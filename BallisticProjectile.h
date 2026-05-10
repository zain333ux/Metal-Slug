#pragma once

#include "Projectile.h"

class BallisticProjectile : public Projectile
{
protected:
	float gravityScale;
	bool exploded;
	float explosionTimer;

	void explode();

public:
	BallisticProjectile();
	BallisticProjectile(float startX, float startY, float startVelocityX, float startVelocityY,
		int newDamage, float newBlastRadius, float newGravityScale, bool playerOwnedProjectile);
	virtual ~BallisticProjectile() = 0;

	virtual void update(float deltaTime) override;
};
