#pragma once

#include "Entity.h"

class Projectile : public Entity
{
protected:
	int damage;
	float lifeTime;
	sf::RectangleShape body;

public:
	Projectile();

	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);

	int getDamage() const;
};
