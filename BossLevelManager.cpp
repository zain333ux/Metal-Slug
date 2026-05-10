#include "BossLevelManager.h"

#include "Collectible.h"
#include "Constants.h"
#include "EntityManager.h"
#include "Level.h"
#include "LevelManager.h"
#include "PlayerSoldier.h"
#include "SlugFlyer.h"
#include "Submarine.h"
#include "Vehicle.h"

#include <iostream>

BossLevelManager::BossLevelManager()
{
	sectionWidth = Constants::BOSS_PLAINS_END_X;
	reset();
}

void BossLevelManager::reset()
{
	currentPhase = BossPhase::GroundIronNokana;
	currentBoss = 0;
	clearBossPointers();
	started = false;
	phaseLocked = false;
	completionScoreAwarded = false;
}

void BossLevelManager::clearBossPointers()
{
	currentBoss = 0;
	for (int i = 0; i < 3; i += 1)
	{
		finalBosses[i] = 0;
		finalBossDefeated[i] = false;
	}
}

void BossLevelManager::startBossLevel(EntityManager& entities, PlayerSoldier* player, LevelManager& level)
{
	reset();
	started = true;
	spawnPhaseBoss(entities, player, level);
	spawnPhaseVehicle(entities, level);
	lockArena(player);
	std::cout << "Boss phase changed: " << getPhaseName() << std::endl;
}

void BossLevelManager::spawnPhaseBoss(EntityManager& entities, PlayerSoldier* player, LevelManager& levelManager)
{
	Level* level = levelManager.getCurrentLevel();
	float startX = phaseStartX(currentPhase);
	float arenaLeft = startX + 50.0f;
	float arenaRight = phaseEndX(currentPhase) - 50.0f;
	float arenaTop = 40.0f;
	float arenaBottom = level != 0 ? level->getWorldHeight() - 20.0f : Constants::WORLD_HEIGHT - 20.0f;
	if (currentPhase == BossPhase::GroundIronNokana)
	{
		currentBoss = new IronNokanaBoss(1118.0f, 1297.0f, false);
		currentBoss->setTarget(player);
		currentBoss->setArenaBounds(arenaLeft, arenaRight, 1297.0f, 1297.0f);
		currentBoss->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
		entities.addEntity(currentBoss);
		std::cout << "Iron Nokana spawned at world position (1118, 1297), HP 300" << std::endl;
	}
	else if (currentPhase == BossPhase::AquaticSeaSatan)
	{
		currentBoss = new SeaSatanBoss(2829.0f, 1154.0f, false);
		currentBoss->setTarget(player);
		currentBoss->setArenaBounds(arenaLeft, arenaRight, 1006.0f, arenaBottom);
		currentBoss->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
		entities.addEntity(currentBoss);
		std::cout << "Sea Satan spawned at world position (2829, 1154), HP 300" << std::endl;
	}
	else if (currentPhase == BossPhase::AerialHairbuster)
	{
		currentBoss = new HairbusterRibertsBoss(4248.0f, 500.0f, false);
		currentBoss->setTarget(player);
		currentBoss->setArenaBounds(arenaLeft, arenaRight, 250.0f, 700.0f);
		currentBoss->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
		entities.addEntity(currentBoss);
		std::cout << "Hairbuster spawned at world position (4248, 500), HP 300" << std::endl;
	}
	else if (currentPhase == BossPhase::FinalMerged)
	{
		finalBosses[0] = new IronNokanaBoss(5000.0f, 1297.0f, true);
		finalBosses[1] = new SeaSatanBoss(5700.0f, 1154.0f, true);
		finalBosses[2] = new HairbusterRibertsBoss(6400.0f, 500.0f, true);
		finalBosses[0]->setArenaBounds(arenaLeft, arenaRight, 1297.0f, 1297.0f);
		finalBosses[1]->setArenaBounds(arenaLeft, arenaRight, 1006.0f, arenaBottom);
		finalBosses[2]->setArenaBounds(arenaLeft, arenaRight, 250.0f, 700.0f);
		for (int i = 0; i < 3; i += 1)
		{
			finalBosses[i]->setTarget(player);
			finalBosses[i]->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
			entities.addEntity(finalBosses[i]);
			std::cout << finalBosses[i]->getBossDisplayName() << " spawned at world position ("
				<< finalBosses[i]->getX() << ", " << finalBosses[i]->getY() << "), HP 300" << std::endl;
		}
	}
}

void BossLevelManager::spawnPhaseVehicle(EntityManager& entities, LevelManager& levelManager)
{
	Level* level = levelManager.getCurrentLevel();
	float startX = phaseStartX(currentPhase);
	float maxX = level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH;

	if (currentPhase == BossPhase::GroundIronNokana)
	{
		float y = level != 0 ? level->getMainGroundYAt(startX + 320.0f) - 96.0f : static_cast<float>(Constants::GROUND_Y) - 96.0f;
		Vehicle* metalSlug = new Vehicle(startX + 320.0f, y);
		metalSlug->setMovementMaxX(maxX);
		metalSlug->setActiveLevel(level);
		entities.addEntity(metalSlug);
	}
	else if (currentPhase == BossPhase::AquaticSeaSatan)
	{
		SlugMariner* mariner = new SlugMariner(startX + 280.0f, 625.0f);
		mariner->setMovementMaxX(maxX);
		mariner->setActiveLevel(level);
		entities.addEntity(mariner);
	}
	else if (currentPhase == BossPhase::AerialHairbuster)
	{
		SlugFlyer* flyer = new SlugFlyer(startX + 280.0f, 330.0f);
		flyer->setMovementMaxX(maxX);
		flyer->setActiveLevel(level);
		entities.addEntity(flyer);
	}
	else if (currentPhase == BossPhase::FinalMerged)
	{
		float y = level != 0 ? level->getMainGroundYAt(startX + 260.0f) - 96.0f : static_cast<float>(Constants::GROUND_Y) - 96.0f;
		Vehicle* metalSlug = new Vehicle(startX + 260.0f, y);
		SlugMariner* mariner = new SlugMariner(startX + 760.0f, 625.0f);
		SlugFlyer* flyer = new SlugFlyer(startX + 1240.0f, 330.0f);
		metalSlug->setMovementMaxX(maxX);
		mariner->setMovementMaxX(maxX);
		flyer->setMovementMaxX(maxX);
		metalSlug->setActiveLevel(level);
		mariner->setActiveLevel(level);
		flyer->setActiveLevel(level);
		entities.addEntity(metalSlug);
		entities.addEntity(mariner);
		entities.addEntity(flyer);
	}
}

void BossLevelManager::lockArena(PlayerSoldier* player)
{
	if (player != 0)
	{
		player->setMovementMaxX(phaseEndX(currentPhase));
	}
	phaseLocked = true;
}

void BossLevelManager::unlockArena(PlayerSoldier* player)
{
	if (player != 0)
	{
		player->setMovementMaxX(phaseEndX(currentPhase) + sectionWidth);
	}
	phaseLocked = false;
}

void BossLevelManager::cleanupPhase(EntityManager& entities)
{
	// Phase transitions remove boss bullets and leftover minions so the next biome starts clean.
	entities.clearBossPhaseTransientEntities();
	currentBoss = 0;
	for (int i = 0; i < 3; i += 1)
	{
		finalBosses[i] = 0;
	}
	entities.addEntity(new Collectible(COLLECTIBLE_TURKEY, phaseEndX(currentPhase) + 80.0f, 650.0f));
}

void BossLevelManager::advancePhase(EntityManager& entities, PlayerSoldier* player, LevelManager& level)
{
	if (currentPhase == BossPhase::GroundIronNokana)
	{
		currentPhase = BossPhase::AquaticSeaSatan;
	}
	else if (currentPhase == BossPhase::AquaticSeaSatan)
	{
		currentPhase = BossPhase::AerialHairbuster;
	}
	else if (currentPhase == BossPhase::AerialHairbuster)
	{
		currentPhase = BossPhase::FinalMerged;
	}
	else
	{
		currentPhase = BossPhase::Completed;
		return;
	}

	spawnPhaseBoss(entities, player, level);
	spawnPhaseVehicle(entities, level);
	lockArena(player);
	std::cout << "Boss phase changed: " << getPhaseName() << std::endl;
}

void BossLevelManager::update(float deltaTime, EntityManager& entities, PlayerSoldier* player, LevelManager& level)
{
	if (!started || player == 0 || currentPhase == BossPhase::Completed)
	{
		return;
	}

	if (currentPhase != BossPhase::FinalMerged)
	{
		if (currentBoss == 0 || !currentBoss->isActive())
		{
			return;
		}

		if (currentBoss->getHealth() <= 0)
		{
			std::cout << currentBoss->getBossDisplayName() << " defeated." << std::endl;
			entities.addBonusScore(500);
			unlockArena(player);
			cleanupPhase(entities);
			advancePhase(entities, player, level);
		}
		else
		{
			currentBoss->update(deltaTime, *player, entities, level);
		}
		return;
	}

	for (int i = 0; i < 3; i += 1)
	{
		if (finalBosses[i] != 0 && finalBosses[i]->isActive())
		{
			if (finalBosses[i]->getHealth() <= 0)
			{
				std::cout << finalBosses[i]->getBossDisplayName() << " defeated." << std::endl;
				entities.addBonusScore(500);
				finalBosses[i]->deactivate();
				finalBosses[i] = 0;
				finalBossDefeated[i] = true;
			}
			else
			{
				finalBosses[i]->update(deltaTime, *player, entities, level);
			}
		}
	}

	if (finalBossesDefeated())
	{
		if (!completionScoreAwarded)
		{
			entities.addBonusScore(1500);
			completionScoreAwarded = true;
		}
		unlockArena(player);
		currentPhase = BossPhase::Completed;
		std::cout << "Boss phase changed: " << getPhaseName() << std::endl;
	}
}

float BossLevelManager::phaseStartX(BossPhase phase) const
{
	if (phase == BossPhase::AquaticSeaSatan)
	{
		return Constants::BOSS_PLAINS_END_X;
	}
	if (phase == BossPhase::AerialHairbuster)
	{
		return Constants::BOSS_AQUATIC_END_X;
	}
	if (phase == BossPhase::FinalMerged)
	{
		return Constants::BOSS_AERIAL_END_X;
	}
	return 0.0f;
}

float BossLevelManager::phaseEndX(BossPhase phase) const
{
	if (phase == BossPhase::GroundIronNokana)
	{
		return Constants::BOSS_PLAINS_END_X;
	}
	if (phase == BossPhase::AquaticSeaSatan)
	{
		return Constants::BOSS_AQUATIC_END_X;
	}
	if (phase == BossPhase::AerialHairbuster)
	{
		return Constants::BOSS_AERIAL_END_X;
	}
	return Constants::BOSS_FINAL_END_X;
}

const char* BossLevelManager::getPhaseName() const
{
	if (currentPhase == BossPhase::GroundIronNokana)
	{
		return "Phase 1 Plains";
	}
	if (currentPhase == BossPhase::AquaticSeaSatan)
	{
		return "Phase 2 Aquatic";
	}
	if (currentPhase == BossPhase::AerialHairbuster)
	{
		return "Phase 3 Aerial";
	}
	if (currentPhase == BossPhase::FinalMerged)
	{
		return "Phase 4 Final Merged";
	}
	return "Boss Level Complete";
}

bool BossLevelManager::finalBossesDefeated() const
{
	for (int i = 0; i < 3; i += 1)
	{
		if (!finalBossDefeated[i])
		{
			return false;
		}
	}
	return true;
}

bool BossLevelManager::isCompleted() const
{
	return currentPhase == BossPhase::Completed;
}

BossPhase BossLevelManager::getCurrentPhase() const
{
	return currentPhase;
}

void BossLevelManager::appendHudText(std::string& text) const
{
	if (!started)
	{
		return;
	}

	text += "\n";
	text += getPhaseName();
	if (currentPhase != BossPhase::FinalMerged)
	{
		if (currentBoss != 0)
		{
			text += "  ";
			text += currentBoss->getBossDisplayName();
			text += " HP ";
			text += std::to_string(currentBoss->getHealth());
			text += "/";
			text += std::to_string(currentBoss->getMaxHealth());
		}
		return;
	}

	for (int i = 0; i < 3; i += 1)
	{
		if (finalBosses[i] != 0)
		{
			text += "  ";
			text += finalBosses[i]->getBossDisplayName();
			text += " ";
			text += std::to_string(finalBosses[i]->getHealth());
		}
		else if (finalBossDefeated[i])
		{
			if (i == 0)
			{
				text += "  Iron Nokana 0";
			}
			else if (i == 1)
			{
				text += "  Sea Satan 0";
			}
			else
			{
				text += "  Hairbuster Riberts 0";
			}
		}
	}
}
