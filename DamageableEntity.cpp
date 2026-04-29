#include "DamageableEntity.h"

DamageableEntity::DamageableEntity()
{
	maxHealth = 1;
	health = maxHealth;
}

void DamageableEntity::takeDamage(int damage)
{
	if (damage <= 0)
	{
		return;
	}

	health -= damage;
	if (health <= 0)
	{
		health = 0;
		deactivate();
	}
}

void DamageableEntity::heal(int amount)
{
	if (amount <= 0)
	{
		return;
	}

	health += amount;
	if (health > maxHealth)
	{
		health = maxHealth;
	}
}

bool DamageableEntity::isDead() const
{
	return health <= 0;
}

int DamageableEntity::getHealth() const
{
	return health;
}
