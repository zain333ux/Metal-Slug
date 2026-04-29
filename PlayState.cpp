#include "PlayState.h"

#include "Enemy.h"
#include "Game.h"
#include "PlayerSoldier.h"

#include <string>

PlayState::PlayState()
{
	initialized = false;
	score = 0;
	player = 0;
}

void PlayState::initialize(Game& game)
{
	player = new PlayerSoldier();
	game.getEntityManager().addEntity(player);

	spawnEnemy(game, 850.0f, 500.0f);
	spawnEnemy(game, 1100.0f, 500.0f);
	spawnEnemy(game, 1350.0f, 500.0f);

	initialized = true;
	updateWindowTitle(game);
}

void PlayState::spawnEnemy(Game& game, float x, float y)
{
	Enemy* enemy = new Enemy();
	enemy->setSpawnPosition(x, y);
	enemy->setTarget(player);
	game.getEntityManager().addEntity(enemy);
}

void PlayState::updateWindowTitle(Game& game)
{
	game.getWindow().setTitle("Metal Slug OOP - Score: " + std::to_string(score));
}

void PlayState::handleInput(Game& game)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		game.close();
	}
}

void PlayState::update(Game& game, float deltaTime)
{
	if (!initialized)
	{
		initialize(game);
	}

	game.getLevelManager().update(deltaTime);
	if (player != 0 && player->isActive())
	{
		player->handleWeaponInput(game.getEntityManager(), deltaTime);
	}
	game.getEntityManager().updateAll(deltaTime);

	int gainedScore = game.getEntityManager().collectPendingScore();
	if (gainedScore > 0)
	{
		score += gainedScore;
		updateWindowTitle(game);
	}
}

void PlayState::draw(Game& game, sf::RenderWindow& window)
{
	game.getLevelManager().draw(window);
	game.getEntityManager().drawAll(window);
}
