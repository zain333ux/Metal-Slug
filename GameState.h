#pragma once

#include <SFML/Graphics.hpp>

class Game;

class GameState
{
public:
	virtual ~GameState();

	virtual void handleInput(Game& game) = 0;
	virtual void update(Game& game, float deltaTime) = 0;
	virtual void draw(Game& game, sf::RenderWindow& window) = 0;
};
