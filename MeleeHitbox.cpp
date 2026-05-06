#include "MeleeHitbox.h"

MeleeHitbox::MeleeHitbox(float startX, float startY, bool facingRight, bool airborne)
{
	damage = 20;
	lifeTime = 0.08f;
	width = 48.0f;
	height = 58.0f;
	melee = true;
	markFiredWhileAirborne(airborne);

	if (!facingRight)
	{
		startX -= width;
	}

	setPosition(startX, startY);
	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(255, 255, 255, 90));
}
