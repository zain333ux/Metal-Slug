#pragma once

#include "Entity.h"

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
};
