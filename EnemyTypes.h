#pragma once

#include "Enemy.h"

class RebelSoldier : public Enemy
{
private:
	int rebelAnimationState;
	float fireTimer;
	float shotReleaseTimer;
	sf::Texture idleTexture;
	sf::Texture runTexture;
	sf::Texture fireTexture;

	void setRebelAnimation(int newState);
	void updateRebelAnimation();

public:
	RebelSoldier();
	void update(float deltaTime);
	Projectile* createProjectileIfReady();
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
private:
	int bazookaState;
	int bazookaAnimationState;
	float hurtTimer;
	float fireTimer;
	float rocketReleaseTimer;
	float reloadTimer;
	float rocketCooldown;
	bool queuedRocket;
	sf::Texture idleTexture;
	sf::Texture runTexture;
	sf::Texture fireTexture;

	void updateAI();
	void updateBazookaAnimation();
	void setBazookaAnimation(int newState);

public:
	BazookaSoldier();
	void update(float deltaTime);
	Projectile* createProjectileIfReady();
	bool applyProjectileHit(Projectile& projectile);
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
private:
	int martianAnimationState;
	sf::Texture idleTexture;
	sf::Texture walkTexture;

	void setMartianAnimation(int newState);
	void updateMartianAnimation();

protected:
	void updateAI();

public:
	MartianEnemy();
	void update(float deltaTime);
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
