#include "EnemyDownwardBullet.h"

#include "Constants.h"
#include "Level.h"

EnemyDownwardBullet::EnemyDownwardBullet(float startX, float startY, Level* level)
{
	damage = 12;
	lifeTime = 2.2f;
	width = 10.0f;
	height = 22.0f;
	activeLevel = level;
	setPlayerOwned(false);
	setPosition(startX, startY);
	setVelocity(0.0f, 520.0f);

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(120, 255, 90));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(1.0f);
}

void EnemyDownwardBullet::update(float deltaTime)
{
	Projectile::update(deltaTime);
	if (!isActive())
	{
		return;
	}

	float landingY = static_cast<float>(Constants::GROUND_Y);
	if (activeLevel != 0)
	{
		landingY = activeLevel->getGroundYAt(getCenterX());
	}

	if (y + height >= landingY)
	{
		deactivate();
	}
}
