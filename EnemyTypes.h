#pragma once

#include "Enemy.h"

class RebelSoldier : public Enemy
{
public:
	RebelSoldier();
	const char* getEnemyName() const;
};

class ShieldedSoldier : public Enemy
{
private:
	int shieldState;
	int shieldAnimationState;
	float hurtTimer;
	float shootTimer;
	float shotReleaseTimer;
	sf::Texture idleTexture;
	sf::Texture runTexture;
	sf::Texture fireTexture;

	bool isHitFromFront(const Projectile& projectile) const;
	bool isHitFromAbove(const Projectile& projectile) const;
	void setShieldAnimation(int newState);
	void updateShieldAnimation();

public:
	ShieldedSoldier();
	void update(float deltaTime);
	Projectile* createProjectileIfReady();
	bool applyProjectileHit(Projectile& projectile);
	const char* getEnemyName() const;
};

class BazookaSoldier : public Enemy
{
public:
	BazookaSoldier();
	const char* getEnemyName() const;
};

class GrenadeSoldier : public Enemy
{
private:
	int grenadeState;
	int grenadeAnimationState;
	float hurtTimer;
	float fireTimer;
	float throwReleaseTimer;
	float grenadeTimer;
	float grenadeCooldown;
	bool queuedGrenade;
	sf::Texture idleTexture;
	sf::Texture runTexture;
	sf::Texture fireTexture;

	void updateAI();
	void updateGrenadeAnimation();
	void setGrenadeAnimation(int newState);

public:
	GrenadeSoldier();
	void update(float deltaTime);
	Projectile* createProjectileIfReady();
	bool applyProjectileHit(Projectile& projectile);
	const char* getEnemyName() const;
};

class ZombieEnemy : public Enemy
{
public:
	ZombieEnemy();
	const char* getEnemyName() const;
};

class MummyEnemy : public Enemy
{
public:
	MummyEnemy();
	const char* getEnemyName() const;
};

class MartianEnemy : public Enemy
{
protected:
	void updateAI();

public:
	MartianEnemy();
	const char* getEnemyName() const;
};

class BossEnemy : public Enemy
{
private:
	int phase;

protected:
	void updateAI();

public:
	BossEnemy(int bossPhase);
	const char* getEnemyName() const;
};
