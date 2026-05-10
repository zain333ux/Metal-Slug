#include "Projectile.h"
#include "AudioManager.h"
#include "Constants.h"

#include "Level.h"


using namespace std;
using namespace sf;

Projectile::Projectile()
{
	damage = 10;
	lifeTime = 2;
	width = 22;
	height = 8;
	explosive = false;
	melee = false;
	firedWhileAirborne = false;
	playerOwned = true;
	blastRadius = 0;
	activeLevel = 0;

	body.setSize(Vector2f(width, height));
	body.setFillColor(Color(255, 230, 90));
	body.setOutlineColor(Color::Black);
	body.setOutlineThickness(1);
}

Projectile::~Projectile()
{
}

void Projectile::update(float deltaTime)
{
	lifeTime -= deltaTime;
	if (lifeTime <= 0)
	{
		deactivate();
		return;
	}

	Entity::update(deltaTime);
	body.setPosition(x, y);
}

void Projectile::draw(RenderWindow& window)
{
	if (visible)
	{
		window.draw(body);
	}
}

void Projectile::onCollision()
{
	if (explosive)
	{
		AudioManager::playGlobalSound(SFX_EXPLOSION);
	}
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
		return activeLevel->getMainGroundYAt(x + width / 2);
	}

	return static_cast<float>(Constants::GROUND_Y);
}
