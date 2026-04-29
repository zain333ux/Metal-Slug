#include "StraightProjectile.h"

#include "Constants.h"

StraightProjectile::StraightProjectile(float startX, float startY, bool facingRight)
{
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(Constants::BULLET_SPEED, 0.0f);
	}
	else
	{
		setVelocity(-Constants::BULLET_SPEED, 0.0f);
	}
}

void StraightProjectile::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0.0f || x > Constants::SCREEN_WIDTH)
	{
		deactivate();
	}
}
