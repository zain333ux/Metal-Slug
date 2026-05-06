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
	int currentCharacter;
	int lives[4];
	int grenades[4];
	int rockets[4];
	float meleeTimer;
	float grenadeTimer;
	float rocketTimer;
	float damageTimer;
	bool aimingUp;
	bool previousSwitchKey;
	bool previousMeleeKey;
	bool previousGrenadeKey;
	bool previousRocketKey;

	void handleInput();
	void updatePlayerAnimation(float deltaTime);
	void applyCharacterStats();

public:
	PlayerSoldier();

	void takeDamage(int damage);
	void respawn();
	void update(float deltaTime);
	void handleWeaponInput(EntityManager& entityManager, float deltaTime);
	void switchCharacter();
	void refillDemoInventory();
	const char* getCharacterName() const;
	int getLives() const;
	int getGrenades() const;
	int getRockets() const;
};
