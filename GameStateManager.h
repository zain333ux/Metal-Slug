#pragma once

#include "GameState.h"

class GameStateManager
{
private:
	GameState* currentState;

public:
	GameStateManager();
	~GameStateManager();

	void changeState(GameState* newState);
	void handleInput(Game& game);
	void update(Game& game, float deltaTime);
	void draw(Game& game, sf::RenderWindow& window);
};
