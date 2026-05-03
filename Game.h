#pragma once
#include "EntityManager.h"
#include "GameStateManager.h"
#include "LevelManager.h"
using namespace sf;


class Game
{
private:
	RenderWindow window;
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
	void changeState(GameState* newState);

	EntityManager& getEntityManager();
	LevelManager& getLevelManager();
	RenderWindow& getWindow();

};
