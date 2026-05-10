#pragma once

#include "Enemy.h"

class EntityManager;
class LevelManager;
class PlayerSoldier;
class Projectile;

enum class BossPhase
{
	GroundIronNokana,
	AquaticSeaSatan,
	AerialHairbuster,
	FinalMerged,
	Completed
};

enum class BossType
{
	IronNokana,
	SeaSatan,
	HairbusterRiberts
};

enum class BiomeType
{
	Plains,
	Aquatic,
	Aerial,
	Merged
};

class Boss : public Enemy
{
protected:
	BossType bossType;
	BiomeType biomeType;
	bool retreating;
	bool retreatFinished;
	bool finalPhase;
	float arenaLeft;
	float arenaRight;
	float arenaTop;
	float arenaBottom;
	float attackTimer;
	float minionSpawnTimer;
	bool waitingForMinionBatchClear;
	int pendingProjectiles[8];
	int pendingProjectileCount;

	bool loadStaticBossTexture(const char* fileName, float scale);
	void enqueueProjectile(int projectileKind);
	void updateBossVisual();
	void updateMinionBatch(EntityManager& entities);
	void clampToArena();
	virtual Projectile* buildProjectile(int projectileKind) = 0;

public:
	Boss(BossType newBossType, BiomeType newBiomeType, bool newFinalPhase);
	virtual ~Boss();

	virtual void update(float deltaTime) override;
	virtual void update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level) = 0;
	virtual void attack(PlayerSoldier& player, EntityManager& entities) = 0;
	virtual void spawnMinions(EntityManager& entities) = 0;
	virtual void startRetreat() = 0;
	virtual void updateRetreat(float deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) override;
	virtual bool shouldRetreat() const;
	virtual bool isBossDead() const;
	virtual void takeDamage(int amount) override;
	virtual bool applyProjectileHit(Projectile& projectile) override;
	virtual Projectile* createProjectileIfReady() override;
	bool hasRetreated() const;
	bool isRetreating() const;
	bool isFinalPhaseBoss() const;
	BossType getBossType() const;
	const char* getBossDisplayName() const;
	int getMaxHealth() const;
	void setArenaBounds(float left, float right, float top, float bottom);
};

class IronNokanaBoss : public Boss
{
private:
	float patrolDirectionTimer;

	virtual Projectile* buildProjectile(int projectileKind) override;

public:
	IronNokanaBoss(float startX, float groundY, bool finalPhaseBoss);
	virtual void update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level) override;
	virtual void attack(PlayerSoldier& player, EntityManager& entities) override;
	virtual void spawnMinions(EntityManager& entities) override;
	virtual void startRetreat() override;
	virtual void updateRetreat(float deltaTime) override;
	virtual const char* getEnemyName() const override;
};

class SeaSatanBoss : public Boss
{
private:
	float patrolLeftBound;
	float patrolRightBound;

	virtual Projectile* buildProjectile(int projectileKind) override;

public:
	SeaSatanBoss(float startX, float startY, bool finalPhaseBoss);
	virtual void update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level) override;
	virtual void attack(PlayerSoldier& player, EntityManager& entities) override;
	virtual void spawnMinions(EntityManager& entities) override;
	virtual void startRetreat() override;
	virtual void updateRetreat(float deltaTime) override;
	virtual const char* getEnemyName() const override;
};

class HairbusterRibertsBoss : public Boss
{
private:
	float patrolLeftBound;
	float patrolRightBound;

	virtual Projectile* buildProjectile(int projectileKind) override;

public:
	HairbusterRibertsBoss(float startX, float startY, bool finalPhaseBoss);
	virtual void update(float deltaTime, PlayerSoldier& player, EntityManager& entities, LevelManager& level) override;
	virtual void attack(PlayerSoldier& player, EntityManager& entities) override;
	virtual void spawnMinions(EntityManager& entities) override;
	virtual void startRetreat() override;
	virtual void updateRetreat(float deltaTime) override;
	virtual const char* getEnemyName() const override;
};
