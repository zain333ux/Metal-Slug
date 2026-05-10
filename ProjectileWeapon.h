#pragma once

#include "Weapon.h"

class ProjectileWeapon : public Weapon
{
public:
	ProjectileWeapon();
	virtual ~ProjectileWeapon() = 0;

	void fire(EntityManager& entityManager, float startX, float startY, bool facingRight);
	void fire(EntityManager& entityManager, float startX, float startY, bool facingRight, bool upward);
	void setCooldown(float newCooldown);
	void restartCooldown();
};

class BasicProjectileWeapon : public ProjectileWeapon
{
public:
	BasicProjectileWeapon();
};
