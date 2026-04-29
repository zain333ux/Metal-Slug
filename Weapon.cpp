#include "Weapon.h"

Weapon::Weapon()
{
	damage = 10;
	cooldown = 0.25f;
	cooldownTimer = 0.0f;
}

Weapon::~Weapon()
{
}

void Weapon::update(float deltaTime)
{
	if (cooldownTimer > 0.0f)
	{
		cooldownTimer -= deltaTime;
	}
}

bool Weapon::canFire() const
{
	return cooldownTimer <= 0.0f;
}
