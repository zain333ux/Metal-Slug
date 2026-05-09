#pragma once

#include "Enemy.h"

class RebelSoldier : public Enemy
{
private:
	int rebelState;
	int rebelAnimationState;
	float hurtTimer;
	float shootTimer;
	float shotReleaseTimer;
	bool queuedRebelShot;
	const sf::IntRect* currentFrames;
	int customFrameCount;
	int customFrameIndex;
	float customFrameTimer;
	float customFrameDuration;
	sf::Texture idleTexture;
	sf::Texture runTexture;
	sf::Texture fireTexture;

	void updateRebelAnimation(float deltaTime);
	void setRebelAnimation(int newState);
	void setCustomAnimation(sf::Texture& newTexture, const sf::IntRect* frames, int frameCount, float frameDuration);
	void updateCustomAnimation(float deltaTime);

public:
	RebelSoldier();
	void update(float deltaTime);
	Projectile* createProjectileIfReady();
	bool applyProjectileHit(Projectile& projectile);
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

class MartianInfantry : public Enemy
{
private:
	int martianState;
	int martianAnimationState;
	float hurtTimer;
	float fireTimer;
	float shotReleaseTimer;
	float pistolReloadTimer;
	bool queuedShotReady;
	bool teleporting;
	bool dying;
	const sf::IntRect* currentFrames;
	int customFrameCount;
	int customFrameIndex;
	bool customAnimationLoop;
	bool customAnimationFinished;
	float customFrameTimer;
	float customFrameDuration;
	sf::Texture idleTexture;
	sf::Texture walkTexture;
	sf::Texture fireTexture;
	sf::Texture deathTexture;
	sf::Texture teleportTexture;

	void updateAI();
	void updateCustomAnimation(float deltaTime);
	void setCustomAnimation(sf::Texture& newTexture, const sf::IntRect* frames, int frameCount, bool loop, float frameDuration);
	void setMartianAnimation(int newState);
	void finishTeleport();

public:
	MartianInfantry(float startX, float startY);
	void update(float deltaTime);
	Projectile* createProjectileIfReady();
	bool applyProjectileHit(Projectile& projectile);
	const char* getEnemyName() const;
};

class MartianPod : public Enemy
{
private:
	int podState;
	int podAnimationState;
	float hoverTimer;
	float hoverBaseY;
	float hurtTimer;
	float attackTimer;
	float bulletReleaseTimer;
	float attackCooldownTimer;
	float spawnDelayTimer;
	bool attackReleased;
	bool dying;
	bool readyToSpawnInfantry;
	int bulletsToFire;
	const sf::IntRect* currentFrames;
	int customFrameCount;
	int customFrameIndex;
	bool customAnimationLoop;
	bool customAnimationFinished;
	float customFrameTimer;
	float customFrameDuration;
	sf::Texture movementTexture;
	sf::Texture attackTexture;
	sf::Texture deathTexture;
	sf::Sprite attackSprite;
	int attackFrameIndex;
	float attackFrameTimer;

 protected:
	void updateAI();
	bool loadPodTexture(sf::Texture& targetTexture, const char* fileName);

public:
	MartianPod();
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	Projectile* createProjectileIfReady();
	Entity* createSpawnedEntityIfReady();
	bool applyProjectileHit(Projectile& projectile);
	const char* getEnemyName() const;
	void setSpawnPosition(float newX, float newY);

private:
	void updateMovement(float deltaTime);
	void updateAttack(float deltaTime);
	void updateCustomAnimation(float deltaTime);
	void updateAttackAnimation(float deltaTime);
	void setCustomAnimation(sf::Texture& newTexture, const sf::IntRect* frames, int frameCount, bool loop, float frameDuration);
	void setPodAnimation(int newState);
	void startDeathAnimation();
	float findTeleportX() const;
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
