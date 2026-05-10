#include "Weapon.h"


using namespace std;

Weapon::Weapon()
{
	damage = 10;
	cooldown = 0.25f;
	cooldownTimer = 0;
}

Weapon::~Weapon()
{
}

void Weapon::update(float deltaTime)
{
	if (cooldownTimer > 0)
	{
		cooldownTimer -= deltaTime;
	}
}

bool Weapon::canFire() const
{
	return cooldownTimer <= 0;
}