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

using namespace std;
using namespace sf;

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
}

void BossLevelManager::spawnPhaseBoss(EntityManager& entities, PlayerSoldier* player, LevelManager& levelManager)
{
	Level* level = levelManager.getCurrentLevel();
	float startX = phaseStartX(currentPhase);
	float arenaLeft = startX + 50;
	float arenaRight = phaseEndX(currentPhase) - 50;
	float arenaTop = 40;
	float arenaBottom = level != 0 ? level->getWorldHeight() - 20 : Constants::WORLD_HEIGHT - 20;
	if (currentPhase == BossPhase::GroundIronNokana)
	{
		// Iron Nokana is the ground wala boss, so we fixing it to the ground
		float ironGroundY = 1297;
		currentBoss = new IronNokanaBoss(1118, ironGroundY, false);
		currentBoss->setTarget(player);
		currentBoss->setArenaBounds(arenaLeft, arenaRight, currentBoss->getY(), currentBoss->getY() + currentBoss->getHeight());
		currentBoss->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
		entities.addEntity(currentBoss);
	}
	else if (currentPhase == BossPhase::AquaticSeaSatan)
	{
		currentBoss = new SeaSatanBoss(2829, 1154, false);
		currentBoss->setTarget(player);
		currentBoss->setArenaBounds(arenaLeft, arenaRight, 1006, arenaBottom);
		currentBoss->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
		entities.addEntity(currentBoss);
	}
	else if (currentPhase == BossPhase::AerialHairbuster)
	{
		currentBoss = new HairbusterRibertsBoss(Constants::BOSS_AQUATIC_END_X + 760, 260, false);
		currentBoss->setTarget(player);
		currentBoss->setArenaBounds(arenaLeft, arenaRight, 180, 560);
		currentBoss->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
		entities.addEntity(currentBoss);
	}
	else if (currentPhase == BossPhase::FinalMerged)
	{
		// final phase mein teeno bosses same manager array se track hote hein
		float ironX = 5000;
		float seaX = 5700;
		float ironGroundY = level != 0 ? level->getGroundYAt(ironX) : 1297;
		float seaGroundY = level != 0 ? level->getGroundYAt(seaX) : 1297;
		finalBosses[0] = new IronNokanaBoss(ironX, ironGroundY, true);
		finalBosses[1] = new SeaSatanBoss(seaX, seaGroundY, true);
		finalBosses[1]->setPosition(seaX, seaGroundY - finalBosses[1]->getHeight());
		finalBosses[2] = new HairbusterRibertsBoss(6400, 260, true);
		finalBosses[0]->setArenaBounds(arenaLeft, arenaRight, finalBosses[0]->getY(), finalBosses[0]->getY() + finalBosses[0]->getHeight());
		finalBosses[1]->setArenaBounds(arenaLeft, arenaRight, finalBosses[1]->getY(), finalBosses[1]->getY() + finalBosses[1]->getHeight());
		finalBosses[2]->setArenaBounds(arenaLeft, arenaRight, 180, 560);
		for (int i = 0; i < 3; i += 1)
		{
			finalBosses[i]->setTarget(player);
			finalBosses[i]->setMovementMaxX(level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH);
			entities.addEntity(finalBosses[i]);
		}
	}
}

void BossLevelManager::spawnPhaseVehicle(EntityManager& entities, LevelManager& levelManager)
{
	Level* level = levelManager.getCurrentLevel();
	// har biome ke liye matching vehicle 
	float startX = phaseStartX(currentPhase);
	float maxX = level != 0 ? level->getWorldWidth() : Constants::BOSS_WORLD_WIDTH;

	if (currentPhase == BossPhase::GroundIronNokana)
	{
		float y = level != 0 ? level->getMainGroundYAt(startX + 320) - 96 : static_cast<float>(Constants::GROUND_Y) - 96;
		MetalSlug* metalSlug = new MetalSlug(startX + 320, y);
		metalSlug->setMovementMaxX(maxX);
		metalSlug->setActiveLevel(level);
		entities.addEntity(metalSlug);
	}
	else if (currentPhase == BossPhase::AquaticSeaSatan)
	{
		SlugMariner* mariner = new SlugMariner(startX + 280, 625);
		mariner->setMovementMaxX(maxX);
		mariner->setActiveLevel(level);
		entities.addEntity(mariner);
	}
	else if (currentPhase == BossPhase::AerialHairbuster)
	{
		SlugFlyer* flyer = new SlugFlyer(startX + 280, 620);
		flyer->setMovementMaxX(maxX);
		flyer->setActiveLevel(level);
		entities.addEntity(flyer);
	}
	else if (currentPhase == BossPhase::FinalMerged)
	{
		float y = level != 0 ? level->getMainGroundYAt(startX + 260) - 96 : static_cast<float>(Constants::GROUND_Y) - 96;
		MetalSlug* metalSlug = new MetalSlug(startX + 260, y);
		SlugMariner* mariner = new SlugMariner(startX + 760, 625);
		SlugFlyer* flyer = new SlugFlyer(startX + 1240, 330);
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
		// prevents the player from going forward untill phase is beaten
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
	// phase change pe purane bullets and enemies remove kr dete hein
	entities.clearBossPhaseTransientEntities();
	currentBoss = 0;
	for (int i = 0; i < 3; i += 1)
	{
		finalBosses[i] = 0;
	}
	entities.addEntity(new ItemCollectible(COLLECTIBLE_TURKEY, phaseEndX(currentPhase) + 80, 650));
}

void BossLevelManager::advancePhase(EntityManager& entities, PlayerSoldier* player, LevelManager& level)
{
	// boss phase hp finish hone ke baad next biome start 
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
}

void BossLevelManager::update(float deltaTime, EntityManager& entities, PlayerSoldier* player, LevelManager& level)
{
	if (!started || player == 0 || currentPhase == BossPhase::Completed)
	{
		return;
	}

	if (currentPhase != BossPhase::FinalMerged)
	{
		// single boss phases simple currentBoss pointer se handle ho jate hein
		if (currentBoss == 0 || !currentBoss->isActive())
		{
			return;
		}

		if (currentBoss->getHealth() <= 0)
		{
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
		// final phase mein bosses independent hp ke sath update hote hein
		if (finalBosses[i] != 0 && finalBosses[i]->isActive())
		{
			if (finalBosses[i]->getHealth() <= 0)
			{
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
	return 0;
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
	// all three flags true hon to boss level complete hota he
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

void BossLevelManager::appendHudText(string& text) const
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
			text += to_string(currentBoss->getHealth());
			text += "/";
			text += to_string(currentBoss->getMaxHealth());
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
			text += to_string(finalBosses[i]->getHealth());
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
