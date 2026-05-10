#pragma once

#include "Boss.h"

#include <string>

class EntityManager;
class LevelManager;
class PlayerSoldier;

class BossLevelManager
{
private:
	BossPhase currentPhase;
	Boss* currentBoss;
	Boss* finalBosses[3];
	bool finalBossDefeated[3];
	bool started;
	bool phaseLocked;
	bool completionScoreAwarded;
	float sectionWidth;

	void clearBossPointers();
	void spawnPhaseBoss(EntityManager& entities, PlayerSoldier* player, LevelManager& level);
	void spawnPhaseVehicle(EntityManager& entities, LevelManager& level);
	void lockArena(PlayerSoldier* player);
	void unlockArena(PlayerSoldier* player);
	void cleanupPhase(EntityManager& entities);
	void advancePhase(EntityManager& entities, PlayerSoldier* player, LevelManager& level);
	float phaseStartX(BossPhase phase) const;
	float phaseEndX(BossPhase phase) const;
	const char* getPhaseName() const;
	bool finalBossesDefeated() const;

public:
	BossLevelManager();

	void reset();
	void startBossLevel(EntityManager& entities, PlayerSoldier* player, LevelManager& level);
	void update(float deltaTime, EntityManager& entities, PlayerSoldier* player, LevelManager& level);
	bool isCompleted() const;
	BossPhase getCurrentPhase() const;
	void appendHudText(std::string& text) const;
};
