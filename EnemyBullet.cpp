#include "EnemyBullet.h"

#include "Constants.h"

EnemyBullet::EnemyBullet(float startX, float startY, bool facingRight)
{
	damage = 8;
	lifeTime = 2.0f;
	width = 18.0f;
	height = 6.0f;
	setPlayerOwned(false);
	setPosition(startX, startY);

	if (facingRight)
	{
		setVelocity(Constants::ENEMY_BULLET_SPEED, 0.0f);
	}
	else
	{
		setVelocity(-Constants::ENEMY_BULLET_SPEED, 0.0f);
	}

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(255, 80, 60));
}

void EnemyBullet::update(float deltaTime)
{
	Projectile::update(deltaTime);

	if (x + width < 0.0f || x > 25000.0f)
	{
		deactivate();
	}
}
