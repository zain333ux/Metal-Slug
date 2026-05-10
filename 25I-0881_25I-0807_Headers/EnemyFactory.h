#pragma once

class Enemy;
class PlayerSoldier;

enum EnemyKind
{
	ENEMY_REBEL,
	ENEMY_SHIELDED,
	ENEMY_BAZOOKA,
	ENEMY_GRENADE,
	ENEMY_ZOMBIE,
	ENEMY_MUMMY,
	ENEMY_MARTIAN,
	ENEMY_FLYING_TARA,
	ENEMY_M15A_BRADLEY,
	ENEMY_SUB_VEHICLE,
	ENEMY_BOSS_1,
	ENEMY_BOSS_2,
	ENEMY_BOSS_3,
	ENEMY_ULTIMATE_BOSS
};

class EnemyFactory
{
public:
	static Enemy* createEnemy(EnemyKind kind, float x, float y, PlayerSoldier* target);
};
