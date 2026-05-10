#pragma once

#include <SFML/Graphics.hpp>

#include "DynamicArray.h"
#include "Entity.h"

class PlayerSoldier;
class Level;
class Vehicle;
class Enemy;

class EntityManager
{
private:
	DynamicArray<Entity*> entities;
	int pendingScore;
	Level* activeLevel;
	int destroyedFlyingTara;
	int destroyedBradley;
	int destroyedEnemySub;

	void checkProjectileEnemyCollisions();
	void checkProjectileVehicleCollisions();
	void checkProjectilePlayerCollisions();
	void checkEnemyVehicleCollisions();
	void checkEnemyPlayerCollisions();
	void checkPlayerCollectibleCollisions();
	void handleEnemyKilled(Enemy* enemy);
	void spawnDropForEnemy(const Enemy& enemy);

public:
	EntityManager();
	~EntityManager();

	void addEntity(Entity* entity);
	void setActiveLevel(Level* level);
	void updateAll(float deltaTime);
	void drawAll(sf::RenderWindow& window);
	int collectPendingScore();
	void addBonusScore(int score);
	int countActiveEnemies() const;
	int countActiveNonBossEnemies() const;
	PlayerSoldier* getPlayer() const;
	Vehicle* getVehicle() const;
	Vehicle* getClosestVehicle(float x, float y) const;
	int getDestroyedFlyingTaraCount() const;
	int getDestroyedBradleyCount() const;
	int getDestroyedEnemySubCount() const;
	void removeEnemiesBehind(float minimumX);
	void clearBossPhaseTransientEntities();
	void removeInactive();
	void clear();
};
