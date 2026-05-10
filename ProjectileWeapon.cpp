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

ProjectileWeapon::~ProjectileWeapon()
{
}

BasicProjectileWeapon::BasicProjectileWeapon()
{
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

	BasicStraightProjectile* projectile = new BasicStraightProjectile(startX, startY, facingRight, upward);
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
