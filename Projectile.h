#pragma once

#include "Entity.h"

class Level;

class Projectile : public Entity
{
protected:
	int damage;
	float lifeTime;
	sf::RectangleShape body;
	bool explosive;
	bool melee;
	bool firedWhileAirborne;
	bool playerOwned;
	float blastRadius;
	Level* activeLevel;

	float getGroundY() const;

public:
	Projectile();

	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);
	virtual void onCollision();

	int getDamage() const;
	bool isExplosive() const;
	bool isMelee() const;
	bool wasFiredWhileAirborne() const;
	bool isPlayerOwned() const;
	float getBlastRadius() const;
	void markFiredWhileAirborne(bool airborne);
	void setPlayerOwned(bool owned);
	void setActiveLevel(Level* level);

	/// When player-owned rockets need smoke / linger after an AoE resolves, skip immediate deactivate.
	virtual bool deferProjectileDeactivateAfterEnemyHit();

	/// Skip projectile vs enemy checks during terminal smoke / effects (Slug Flyer rocket).
	virtual bool shouldIgnorePlayerProjectileVsEnemyChecks() const;
};
