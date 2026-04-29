#pragma once

#include <SFML/Graphics.hpp>

#include "EntityManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"

class Game
{
private:
	sf::RenderWindow window;
	bool running;
	GameStateManager gameStateManager;
	EntityManager entityManager;
	LevelManager levelManager;

	void processEvents();
	void update(float deltaTime);
	void render();

public:
	Game();

	void initialize();
	void run();
	void close();

	EntityManager& getEntityManager();
	LevelManager& getLevelManager();
	sf::RenderWindow& getWindow();
};
