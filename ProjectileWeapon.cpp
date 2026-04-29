#include "ProjectileWeapon.h"

#include "Constants.h"
#include "EntityManager.h"
#include "StraightProjectile.h"

ProjectileWeapon::ProjectileWeapon()
{
	damage = 10;
	cooldown = Constants::PLAYER_FIRE_COOLDOWN;
}

void ProjectileWeapon::fire(EntityManager& entityManager, float startX, float startY, bool facingRight)
{
	if (!canFire())
	{
		return;
	}

	StraightProjectile* projectile = new StraightProjectile(startX, startY, facingRight);
	entityManager.addEntity(projectile);
	cooldownTimer = cooldown;
}
