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
public:
	ShieldedSoldier();
	void takeDamage(int damage);
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
public:
	GrenadeSoldier();
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
