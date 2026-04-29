#pragma once

#include "Soldier.h"

class PlayerSoldier;

class Enemy : public Soldier
{
private:
	int scoreValue;
	int contactDamage;
	float stopDistance;
	float contactDamageCooldown;
	float contactDamageTimer;
	PlayerSoldier* target;

	void updateAI();

public:
	Enemy();

	void setSpawnPosition(float newX, float newY);
	void setTarget(PlayerSoldier* newTarget);
	void update(float deltaTime);
	int getScoreValue() const;
	int getContactDamage() const;
	bool canDealContactDamage() const;
	void restartContactDamageCooldown();
};
