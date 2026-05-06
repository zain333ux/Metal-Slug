#pragma once

#include <SFML/Graphics.hpp>

#include "DynamicArray.h"
#include "Entity.h"

class PlayerSoldier;
class Level;

class EntityManager
{
private:
	DynamicArray<Entity*> entities;
	int pendingScore;
	Level* activeLevel;

	void checkProjectileEnemyCollisions();
	void checkProjectilePlayerCollisions();
	void checkEnemyPlayerCollisions();
	void checkPlayerCollectibleCollisions();

public:
	EntityManager();
	~EntityManager();

	void addEntity(Entity* entity);
	void setActiveLevel(Level* level);
	void updateAll(float deltaTime);
	void drawAll(sf::RenderWindow& window);
	int collectPendingScore();
	int countActiveEnemies() const;
	PlayerSoldier* getPlayer() const;
	void removeEnemiesBehind(float minimumX);
	void removeInactive();
	void clear();
};
