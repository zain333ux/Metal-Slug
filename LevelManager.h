#pragma once

#include <SFML/Graphics.hpp>

#include "Level.h"

class LevelManager
{
private:
	Level* currentLevel;

public:
	LevelManager();
	~LevelManager();

	void loadLevel(Level* newLevel);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	Level* getCurrentLevel();
	void clear();
};
