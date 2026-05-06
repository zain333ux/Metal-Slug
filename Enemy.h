#pragma once

#include "Soldier.h"

class PlayerSoldier;
class Projectile;

class Enemy : public Soldier
{
protected:
	int scoreValue;
	int contactDamage;
	float stopDistance;
	float contactDamageCooldown;
	float contactDamageTimer;
	float patrolLeft;
	float patrolRight;
	int patrolDirection;
	float detectionRange;
	float attackRange;
	float shootingRange;
	float pistolCooldown;
	float pistolTimer;
	bool pistolEquipped;
	bool queuedShot;
	PlayerSoldier* target;
	sf::Color fallbackColor;
	bool canMoveInAir;

	virtual void updateAI();

public:
	Enemy();

	void setSpawnPosition(float newX, float newY);
	void setTarget(PlayerSoldier* newTarget);
	void update(float deltaTime);
	Projectile* createProjectileIfReady();
	int getScoreValue() const;
	int getContactDamage() const;
	bool canDealContactDamage() const;
	void restartContactDamageCooldown();
	virtual const char* getEnemyName() const;
};
