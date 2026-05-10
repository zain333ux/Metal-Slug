#pragma once

#include "Entity.h"

class DamageableEntity : public Entity
{
protected:
	int health;
	int maxHealth;

public:
	DamageableEntity();
	virtual ~DamageableEntity() = 0;

	virtual void takeDamage(int damage);
	virtual void heal(int amount);

	bool isDead() const;
	int getHealth() const;
};
