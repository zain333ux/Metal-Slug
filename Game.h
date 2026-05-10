#pragma once
#include "AudioManager.h"
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
	AudioManager audioManager;

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
	AudioManager& getAudioManager();
	sf::RenderWindow& getWindow();

};
