#include "GrenadeProjectile.h"

#include "Constants.h"

GrenadeProjectile::GrenadeProjectile(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 20;
	lifeTime = 2.4f;
	width = 18.0f;
	height = 18.0f;
	explosive = true;
	blastRadius = 180.0f;
	markFiredWhileAirborne(airborne);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(450.0f, -620.0f);
	}
	else
	{
		setVelocity(-450.0f, -620.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(80, 220, 90));
}

void GrenadeProjectile::update(float deltaTime)
{
	velocityY += Constants::GRAVITY * 0.8f * deltaTime;
	Projectile::update(deltaTime);

	if (y + height >= Constants::GROUND_Y)
	{
		deactivate();
	}
}
