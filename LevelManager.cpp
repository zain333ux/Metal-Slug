#include "LevelManager.h"


using namespace std;
using namespace sf;

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
	// old level delete hota he before new one is loaded
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

void LevelManager::draw(RenderWindow& window)
{
	if (currentLevel != 0)
	{
		currentLevel->draw(window);
	}
}

Level* LevelManager::getCurrentLevel()
{
	return currentLevel;
}

void LevelManager::clear()
{
	// LevelManager owns currentLevel so delete yahin hota he
	delete currentLevel;
	currentLevel = 0;
}
