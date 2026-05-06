#include "EnemyFactory.h"

#include "Enemy.h"
#include "EnemyTypes.h"

Enemy* EnemyFactory::createEnemy(EnemyKind kind, float x, float y, PlayerSoldier* target)
{
	Enemy* enemy = 0;

	if (kind == ENEMY_SHIELDED)
	{
		enemy = new ShieldedSoldier();
	}
	else if (kind == ENEMY_BAZOOKA)
	{
		enemy = new BazookaSoldier();
	}
	else if (kind == ENEMY_GRENADE)
	{
		enemy = new GrenadeSoldier();
	}
	else if (kind == ENEMY_ZOMBIE)
	{
		enemy = new ZombieEnemy();
	}
	else if (kind == ENEMY_MUMMY)
	{
		enemy = new MummyEnemy();
	}
	else if (kind == ENEMY_MARTIAN)
	{
		enemy = new MartianEnemy();
	}
	else if (kind == ENEMY_BOSS_1)
	{
		enemy = new BossEnemy(1);
	}
	else if (kind == ENEMY_BOSS_2)
	{
		enemy = new BossEnemy(2);
	}
	else if (kind == ENEMY_BOSS_3)
	{
		enemy = new BossEnemy(3);
	}
	else if (kind == ENEMY_ULTIMATE_BOSS)
	{
		enemy = new BossEnemy(4);
	}
	else
	{
		enemy = new RebelSoldier();
	}

	enemy->setSpawnPosition(x, y);
	enemy->setTarget(target);
	return enemy;
}
