#pragma once

#include "GameState.h"

class PlayerSoldier;

class PlayState : public GameState
{
private:
	bool initialized;
	int score;
	PlayerSoldier* player;

	void initialize(Game& game);
	void spawnEnemy(Game& game, float x, float y);
	void updateWindowTitle(Game& game);

public:
	PlayState();

	void handleInput(Game& game);
	void update(Game& game, float deltaTime);
	void draw(Game& game, sf::RenderWindow& window);
};
