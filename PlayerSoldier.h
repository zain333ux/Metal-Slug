#pragma once

#include "ProjectileWeapon.h"
#include "Soldier.h"

class EntityManager;

class PlayerSoldier : public Soldier
{
private:
	ProjectileWeapon weapon;
	bool firing;
	float fireAnimationTimer;

	void handleInput();
	void updatePlayerAnimation(float deltaTime);

public:
	PlayerSoldier();

	void takeDamage(int damage);
	void respawn();
	void update(float deltaTime);
	void handleWeaponInput(EntityManager& entityManager, float deltaTime);
};
