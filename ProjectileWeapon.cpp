#include "ProjectileWeapon.h"

#include "Constants.h"
#include "EntityManager.h"
#include "StraightProjectile.h"


using namespace std;
using namespace sf;

ProjectileWeapon::ProjectileWeapon()
{
	damage = 10;
	cooldown = Constants::PLAYER_FIRE_COOLDOWN;
}

void ProjectileWeapon::fire(EntityManager& entityManager, float startX, float startY, bool facingRight)
{
	fire(entityManager, startX, startY, facingRight, false);
}

void ProjectileWeapon::fire(EntityManager& entityManager, float startX, float startY, bool facingRight, bool upward)
{
	if (!canFire())
	{
		return;
	}

	StraightProjectile* projectile = new StraightProjectile(startX, startY, facingRight, upward);
	entityManager.addEntity(projectile);
	cooldownTimer = cooldown;
}

void ProjectileWeapon::setCooldown(float newCooldown)
{
	if (newCooldown > 0.02f)
	{
		cooldown = newCooldown;
	}
}

void ProjectileWeapon::restartCooldown()
{
	cooldownTimer = cooldown;
}