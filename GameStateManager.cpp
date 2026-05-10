#include "GameStateManager.h"


using namespace std;
using namespace sf;

GameStateManager::GameStateManager()
{
	currentState = 0;
}

GameStateManager::~GameStateManager()
{
	delete currentState;
}

void GameStateManager::changeState(GameState* newState)
{
	if (currentState != newState)
	{
		delete currentState;
		currentState = newState;
	}
}

void GameStateManager::handleInput(Game& game)
{
	if (currentState != 0)
	{
		currentState->handleInput(game);
	}
}

void GameStateManager::update(Game& game, float deltaTime)
{
	if (currentState != 0)
	{
		currentState->update(game, deltaTime);
	}
}

void GameStateManager::draw(Game& game, RenderWindow& window)
{
	if (currentState != 0)
	{
		currentState->draw(game, window);
	}
}