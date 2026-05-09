#include "Projectile.h"
#include "Constants.h"

#include "Level.h"

Projectile::Projectile()
{
	damage = 10;
	lifeTime = 2.0f;
	width = 22.0f;
	height = 8.0f;
	explosive = false;
	melee = false;
	firedWhileAirborne = false;
	playerOwned = true;
	blastRadius = 0.0f;
	activeLevel = 0;

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(255, 230, 90));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(1.0f);
}

void Projectile::update(float deltaTime)
{
	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		deactivate();
		return;
	}

	Entity::update(deltaTime);
	body.setPosition(x, y);
}

void Projectile::draw(sf::RenderWindow& window)
{
	if (visible)
	{
		window.draw(body);
	}
}

void Projectile::onCollision()
{
	deactivate();
}

int Projectile::getDamage() const
{
	return damage;
}

bool Projectile::isExplosive() const
{
	return explosive;
}

bool Projectile::isMelee() const
{
	return melee;
}

bool Projectile::wasFiredWhileAirborne() const
{
	return firedWhileAirborne;
}

bool Projectile::isPlayerOwned() const
{
	return playerOwned;
}

float Projectile::getBlastRadius() const
{
	return blastRadius;
}

void Projectile::markFiredWhileAirborne(bool airborne)
{
	firedWhileAirborne = airborne;
}

void Projectile::setPlayerOwned(bool owned)
{
	playerOwned = owned;
}

bool Projectile::deferProjectileDeactivateAfterEnemyHit()
{
	return false;
}

bool Projectile::shouldIgnorePlayerProjectileVsEnemyChecks() const
{
	return false;
}

void Projectile::setActiveLevel(Level* level)
{
	activeLevel = level;
}

float Projectile::getGroundY() const
{
	if (activeLevel != 0)
	{
		return activeLevel->getMainGroundYAt(x + width / 2.0f);
	}

	return static_cast<float>(Constants::GROUND_Y);
}
