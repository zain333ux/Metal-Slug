#pragma once

#include <SFML/Graphics.hpp>

#include "DynamicArray.h"
#include "Entity.h"

class EntityManager
{
private:
	DynamicArray<Entity*> entities;
	int pendingScore;

	void checkProjectileEnemyCollisions();
	void checkEnemyPlayerCollisions();

public:
	EntityManager();
	~EntityManager();

	void addEntity(Entity* entity);
	void updateAll(float deltaTime);
	void drawAll(sf::RenderWindow& window);
	int collectPendingScore();
	void removeInactive();
	void clear();
};
