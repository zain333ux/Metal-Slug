#include "RocketProjectile.h"

#include "Constants.h"

RocketProjectile::RocketProjectile(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 35;
	lifeTime = 2.0f;
	width = 34.0f;
	height = 12.0f;
	explosive = true;
	blastRadius = 190.0f;
	markFiredWhileAirborne(airborne);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(650.0f, 0.0f);
	}
	else
	{
		setVelocity(-650.0f, 0.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(255, 120, 50));
}

void RocketProjectile::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0.0f || x > Constants::WORLD_WIDTH_LEVEL_3 + 800.0f)
	{
		deactivate();
	}
}
