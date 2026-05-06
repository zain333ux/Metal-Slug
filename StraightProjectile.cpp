#include "StraightProjectile.h"

#include "Constants.h"

StraightProjectile::StraightProjectile(float startX, float startY, bool facingRight)
	: StraightProjectile(startX, startY, facingRight, false)
{
}

StraightProjectile::StraightProjectile(float startX, float startY, bool facingRight, bool upward)
{
	setPosition(startX, startY);

	if (upward)
	{
		setVelocity(0.0f, -Constants::PLAYER_BULLET_SPEED);
	}
	else if (facingRight)
	{
		setVelocity(Constants::PLAYER_BULLET_SPEED, 0.0f);
	}
	else
	{
		setVelocity(-Constants::PLAYER_BULLET_SPEED, 0.0f);
	}
}

void StraightProjectile::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0.0f || x > 25000.0f || y + height < 0.0f)
	{
		deactivate();
	}
}
