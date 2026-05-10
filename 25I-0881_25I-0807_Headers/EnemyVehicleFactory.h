#pragma once

class Enemy;
class EntityManager;
class Level;
class PlayerSoldier;

class EnemyVehicleFactory
{
public:
	static Enemy* createFlyingTara(float x, float y, PlayerSoldier* target);
	static Enemy* createM15ABradley(float x, float y, PlayerSoldier* target);
	static Enemy* createEnemySub(float x, float y, PlayerSoldier* target);

	static void spawnEnemyVehiclesForSurvivalLevel(int levelNumber, Level* level, EntityManager& entityManager, PlayerSoldier* target);
	static void spawnEnemyVehiclesForCampaign(Level* level, EntityManager& entityManager, PlayerSoldier* target,
		int destroyedFlyingTara, int destroyedBradley, int destroyedSub);
};
