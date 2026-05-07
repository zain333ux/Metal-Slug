#include "EnemyGrenadeProjectile.h"

#include "Constants.h"

EnemyGrenadeProjectile::EnemyGrenadeProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY)
{
	damage = 18;
	lifeTime = 2.6f;
	width = 18.0f;
	height = 18.0f;
	explosive = true;
	blastRadius = 120.0f;
	exploded = false;
	explosionTimer = 0.0f;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(throwVelocityX, throwVelocityY);

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(80, 220, 90));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(1.0f);
}

void EnemyGrenadeProjectile::explode()
{
	if (exploded)
	{
		return;
	}

	exploded = true;
	explosionTimer = 0.16f;
	velocityX = 0.0f;
	velocityY = 0.0f;

	float centerX = getCenterX();
	float centerY = getCenterY();
	width = blastRadius * 2.0f;
	height = blastRadius * 2.0f;
	setPosition(centerX - blastRadius, centerY - blastRadius);
	body.setSize(sf::Vector2f(width, height));
	body.setPosition(x, y);
	visible = false;
	body.setFillColor(sf::Color::Transparent);
}

void EnemyGrenadeProjectile::update(float deltaTime)
{
	if (exploded)
	{
		explosionTimer -= deltaTime;
		if (explosionTimer <= 0.0f)
		{
			deactivate();
		}
		body.setPosition(x, y);
		return;
	}

	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		explode();
		return;
	}

	velocityY += Constants::GRAVITY * 0.75f * deltaTime;
	Entity::update(deltaTime);
	body.setPosition(x, y);

	if (y + height >= Constants::GROUND_Y)
	{
		y = Constants::GROUND_Y - height;
		explode();
	}
}
