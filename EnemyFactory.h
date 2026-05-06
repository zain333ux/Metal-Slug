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
