#pragma once

#include "Enemy.h"

class Projectile;

enum EnemyVehicleType
{
	ENEMY_VEHICLE_FLYING_TARA,
	ENEMY_VEHICLE_M15A_BRADLEY,
	ENEMY_VEHICLE_SUB
};

class EnemyVehicle : public Enemy
{
protected:
	float attackCooldown;
	float attackTimer;
	bool queuedAttack;

public:
	EnemyVehicle();

	virtual void update(float deltaTime) override;
	virtual Projectile* createProjectileIfReady() override;
	virtual Projectile* attack() = 0;
	virtual const char* getType() const = 0;
	virtual EnemyVehicleType getVehicleType() const = 0;
};

class GroundVehicleEnemy : public EnemyVehicle
{
};

class AerialVehicleEnemy : public EnemyVehicle
{
};

class AquaticVehicleEnemy : public EnemyVehicle
{
};
