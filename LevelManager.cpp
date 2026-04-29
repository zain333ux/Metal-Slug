#include "LevelManager.h"

LevelManager::LevelManager()
{
	currentLevel = 0;
}

LevelManager::~LevelManager()
{
	clear();
}

void LevelManager::loadLevel(Level* newLevel)
{
	clear();
	currentLevel = newLevel;

	if (currentLevel != 0)
	{
		currentLevel->load();
	}
}

void LevelManager::update(float deltaTime)
{
	if (currentLevel != 0)
	{
		currentLevel->update(deltaTime);
	}
}

void LevelManager::draw(sf::RenderWindow& window)
{
	if (currentLevel != 0)
	{
		currentLevel->draw(window);
	}
}

void LevelManager::clear()
{
	delete currentLevel;
	currentLevel = 0;
}
