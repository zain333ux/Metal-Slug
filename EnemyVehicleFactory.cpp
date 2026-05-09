#include "EnemyVehicleFactory.h"

#include "EnemySub.h"
#include "EnemyVehicle.h"
#include "EntityManager.h"
#include "FlyingTara.h"
#include "Level.h"
#include "M15ABradley.h"
#include "PlayerSoldier.h"

namespace
{
	const int FLYING_TARA_BATCH_SIZE = 2;

	float clampToPlayableX(Level* level, float x)
	{
		if (level == 0)
		{
			return x;
		}
		if (x < 80.0f)
		{
			return 80.0f;
		}
		if (x > level->getWorldWidth() - 120.0f)
		{
			return level->getWorldWidth() - 120.0f;
		}
		return x;
	}
}

Enemy* EnemyVehicleFactory::createFlyingTara(float x, float y, PlayerSoldier* target)
{
	FlyingTara* enemy = new FlyingTara();
	enemy->setSpawnPosition(x, y);
	enemy->setTarget(target);
	return enemy;
}

Enemy* EnemyVehicleFactory::createM15ABradley(float x, float y, PlayerSoldier* target)
{
	M15ABradley* enemy = new M15ABradley();
	enemy->setSpawnPosition(x, y);
	enemy->setTarget(target);
	return enemy;
}

Enemy* EnemyVehicleFactory::createEnemySub(float x, float y, PlayerSoldier* target)
{
	EnemySub* enemy = new EnemySub();
	enemy->setSpawnPosition(x, y);
	enemy->setTarget(target);
	return enemy;
}

void EnemyVehicleFactory::spawnEnemyVehiclesForSurvivalLevel(int levelNumber, Level* level, EntityManager& entityManager, PlayerSoldier* target)
{
	if (level == 0 || target == 0 || levelNumber < 1 || levelNumber > 3)
	{
		return;
	}

	int taraBatches = 2;
	if (levelNumber >= 2)
	{
		taraBatches = 3;
	}

	for (int batch = 0; batch < taraBatches; batch += 1)
	{
		float baseX = level->getWorldWidth() * (0.30f + 0.18f * static_cast<float>(batch));
		for (int i = 0; i < FLYING_TARA_BATCH_SIZE; i += 1)
		{
			float taraX = clampToPlayableX(level, baseX + static_cast<float>(i) * 120.0f);
			float taraY = level->getMainGroundYAt(taraX) - 280.0f;
			if (taraY < 100.0f)
			{
				taraY = 100.0f;
			}
			entityManager.addEntity(createFlyingTara(taraX, taraY, target));
		}
	}

	if (levelNumber >= 2)
	{
		float bradleyXs[2] = { level->getWorldWidth() * 0.42f, level->getWorldWidth() * 0.68f };
		for (int i = 0; i < 2; i += 1)
		{
			float x = clampToPlayableX(level, bradleyXs[i]);
			float y = level->getMainGroundYAt(x) - 84.0f;
			if (level->isPlainsBiome(x, y + 84.0f))
			{
				entityManager.addEntity(createM15ABradley(x, y, target));
			}
		}
	}

	int subCount = levelNumber >= 3 ? 2 : 1;
	for (int i = 0; i < subCount; i += 1)
	{
		float x = level->getWorldWidth() * (0.72f + static_cast<float>(i) * 0.12f);
		x = clampToPlayableX(level, x);
		float waterY = level->getWaterSurfaceYAt(x);
		float y = waterY + 58.0f;
		if (level->isAquaticBiome(x, y))
		{
			entityManager.addEntity(createEnemySub(x, y, target));
		}
	}

}

void EnemyVehicleFactory::spawnEnemyVehiclesForCampaign(Level* level, EntityManager& entityManager, PlayerSoldier* target,
	int destroyedFlyingTara, int destroyedBradley, int destroyedSub)
{
	if (level == 0 || target == 0)
	{
		return;
	}

	if (destroyedFlyingTara < 3)
	{
		float x = clampToPlayableX(level, target->getX() + 760.0f);
		float y = level->getMainGroundYAt(x) - 260.0f;
		if (level->isAerialBiome(x, y))
		{
			entityManager.addEntity(createFlyingTara(x, y, target));
		}
	}
	if (destroyedBradley < 3)
	{
		float x = clampToPlayableX(level, target->getX() + 960.0f);
		float y = level->getMainGroundYAt(x) - 84.0f;
		if (level->isPlainsBiome(x, y + 84.0f))
		{
			entityManager.addEntity(createM15ABradley(x, y, target));
		}
	}
	if (destroyedSub < 3)
	{
		float x = clampToPlayableX(level, target->getX() + 1160.0f);
		float y = level->getWaterSurfaceYAt(x) + 58.0f;
		if (level->isAquaticBiome(x, y))
		{
			entityManager.addEntity(createEnemySub(x, y, target));
		}
	}

}
