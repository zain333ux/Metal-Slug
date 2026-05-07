#include "PlayState.h"

#include "Collectible.h"
#include "Constants.h"
#include "Enemy.h"
#include "EnemyFactory.h"
#include "Game.h"
#include "Level.h"
#include "LevelProfile.h"
#include "PlayerSoldier.h"
#include "DeveloperMode.h"
#include "ModeSelectState.h"

#include <string>

PlayState::PlayState(PlayMode newMode, int newCampaignProfileOption)
{
	initialized = false;
	levelComplete = false;
	waitingForContinue = false;
	gameOver = false;
	mode = newMode;
	currentLevel = 1;
	campaignProfileOption = newCampaignProfileOption;
	campaignSpawnX = 1200.0f;
	campaignSpawnTimer = 0.0f;
	campaignKills = 0;
	player = 0;
}

void PlayState::initialize(Game& game)
{
	configureHud();
	scoreManager.reset();
	loadCurrentLevel(game);
	initialized = true;
	updateWindowTitle(game);
}

void PlayState::configureHud()
{
	hudFont.loadFromFile("Fonts/PressStart2P.ttf");
	hudText.setFont(hudFont);
	hudText.setCharacterSize(18);
	hudText.setFillColor(sf::Color::White);
	hudText.setPosition(18.0f, 18.0f);

	centerText.setFont(hudFont);
	centerText.setCharacterSize(28);
	centerText.setFillColor(sf::Color(255, 240, 130));
	centerText.setPosition(320.0f, 360.0f);
}

void PlayState::loadCurrentLevel(Game& game)
{
	game.getEntityManager().clear();

	player = new PlayerSoldier();
	game.getEntityManager().addEntity(player);

	if (mode == PLAY_MODE_CAMPAIGN)
	{
		LevelProfile* profile = LevelProfileFactory::createProfile(campaignProfileOption);
		game.getLevelManager().loadLevel(new Level(1, profile->getWorldWidth(), true));
		delete profile;
		spawnCampaignWave(game);
	}
	else if (currentLevel == 4)
	{
		game.getLevelManager().loadLevel(new Level(4, Constants::BOSS_WORLD_WIDTH, false));
		spawnBossWave(game);
	}
	else
	{
		float width = Constants::WORLD_WIDTH_LEVEL_1;
		if (currentLevel == 2)
		{
			width = Constants::WORLD_WIDTH_LEVEL_2;
		}
		else if (currentLevel == 3)
		{
			width = Constants::WORLD_WIDTH_LEVEL_3;
		}
		game.getLevelManager().loadLevel(new Level(currentLevel, width, false));
		spawnSurvivalWave(game);
	}

	Level* loadedLevel = game.getLevelManager().getCurrentLevel();
	if (loadedLevel != 0)
	{
		game.getEntityManager().setActiveLevel(loadedLevel);
		player->setMovementMaxX(loadedLevel->getWorldWidth());
	}

	spawnPickups(game);
	levelComplete = false;
	waitingForContinue = false;
	gameOver = false;
}

void PlayState::spawnEnemy(Game& game, EnemyKind kind, float x, float y)
{
	Enemy* enemy = EnemyFactory::createEnemy(kind, x, y, player);
	Level* level = game.getLevelManager().getCurrentLevel();
	bool flyingEnemy = kind == ENEMY_MARTIAN || kind == ENEMY_BOSS_2;
	if (level != 0 && !flyingEnemy)
	{
		enemy->setPosition(x, level->getMainGroundYAt(x) - enemy->getHeight());
	}
	game.getEntityManager().addEntity(enemy);
}

void PlayState::spawnEnemyAt(Game& game, EnemyKind kind, float x, float y)
{
	Enemy* enemy = EnemyFactory::createEnemy(kind, x, y, player);
	game.getEntityManager().addEntity(enemy);
}

void PlayState::spawnSurvivalWave(Game& game)
{
	float base = static_cast<float>(currentLevel - 1) * 160.0f;
	if (currentLevel == 1)
	{
		spawnEnemy(game, ENEMY_SHIELDED, 700.0f, 500.0f);
		spawnEnemy(game, ENEMY_SHIELDED, 940.0f, 500.0f);
		spawnEnemy(game, ENEMY_GRENADE, 1260.0f, 500.0f);
		spawnEnemyAt(game, ENEMY_GRENADE, 1760.0f, 615.0f - 96.0f);
	}
	spawnEnemy(game, ENEMY_REBEL, 520.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_REBEL, 1160.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_REBEL, 1500.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_SHIELDED, 2180.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_BAZOOKA, 2500.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_GRENADE, 2800.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_ZOMBIE, 3080.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_MUMMY, 3320.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_MARTIAN, 3480.0f + base, 360.0f);
	if (currentLevel >= 2)
	{
		spawnEnemy(game, ENEMY_REBEL, 3600.0f, 500.0f);
		spawnEnemy(game, ENEMY_REBEL, 3690.0f, 500.0f);
		spawnEnemy(game, ENEMY_GRENADE, 3960.0f, 500.0f);
	}
	if (currentLevel >= 3)
	{
		spawnEnemy(game, ENEMY_SHIELDED, 4380.0f, 500.0f);
		spawnEnemy(game, ENEMY_MARTIAN, 4740.0f, 360.0f);
	}
}

void PlayState::spawnBossWave(Game& game)
{
	spawnEnemy(game, ENEMY_BOSS_1, 900.0f, 430.0f);
	spawnEnemy(game, ENEMY_BOSS_2, 1450.0f, 360.0f);
	spawnEnemy(game, ENEMY_BOSS_3, 1900.0f, 430.0f);
	spawnEnemy(game, ENEMY_ULTIMATE_BOSS, 2300.0f, 430.0f);
}

void PlayState::spawnCampaignWave(Game& game)
{
	int activeEnemies = game.getEntityManager().countActiveEnemies();
	const int enemyCap = 10;

	for (int i = 0; i < 3 && activeEnemies < enemyCap; i += 1)
	{
		float x = campaignSpawnX + static_cast<float>(i) * 480.0f;

		if (campaignKills > 18 && i == 2)
		{
			spawnEnemy(game, ENEMY_MARTIAN, x, 360.0f);
		}
		else if (campaignKills > 10 && i == 1)
		{
			spawnEnemy(game, ENEMY_GRENADE, x + 320.0f, 500.0f);
		}
		else if (campaignKills > 5 && i == 1)
		{
			spawnEnemy(game, ENEMY_SHIELDED, x + 160.0f, 500.0f);
		}
		else
		{
			spawnEnemy(game, ENEMY_REBEL, x, 500.0f);
			if (activeEnemies + 1 < enemyCap)
			{
				spawnEnemy(game, ENEMY_REBEL, x + 85.0f, 500.0f);
				activeEnemies += 1;
			}
		}

		activeEnemies += 1;
	}

	campaignSpawnX += 1450.0f;
}

void PlayState::updateCampaignSpawning(Game& game, float deltaTime)
{
	if (mode != PLAY_MODE_CAMPAIGN || player == 0)
	{
		return;
	}

	Level* level = game.getLevelManager().getCurrentLevel();
	if (level != 0)
	{
		level->extendIfNeeded(player->getX());
		player->setMovementMaxX(level->getWorldWidth());
	}

	game.getEntityManager().removeEnemiesBehind(player->getX() - 1300.0f);

	if (campaignSpawnTimer > 0.0f)
	{
		campaignSpawnTimer -= deltaTime;
	}

	if (campaignSpawnTimer <= 0.0f && game.getEntityManager().countActiveEnemies() < 10)
	{
		if (campaignSpawnX < player->getX() + 900.0f)
		{
			campaignSpawnX = player->getX() + 900.0f;
		}
		spawnCampaignWave(game);
		game.getEntityManager().setActiveLevel(level);
		campaignSpawnTimer = 2.0f;
	}
}

void PlayState::spawnPickups(Game& game)
{
	Level* level = game.getLevelManager().getCurrentLevel();
	float crateX = 620.0f;
	float powX = 1760.0f;
	float foodX = 2500.0f;
	float crateY = 700.0f;
	float powY = 700.0f;
	float foodY = 700.0f;

	if (level != 0)
	{
		crateY = level->getMainGroundYAt(crateX) - 42.0f;
		powY = level->getMainGroundYAt(powX) - 42.0f;
		foodY = level->getMainGroundYAt(foodX) - 42.0f;
	}

	game.getEntityManager().addEntity(new Collectible(COLLECTIBLE_SUPPLY_CRATE, crateX, crateY));
	game.getEntityManager().addEntity(new Collectible(COLLECTIBLE_POW, powX, powY));
	game.getEntityManager().addEntity(new Collectible(COLLECTIBLE_FOOD, foodX, foodY));
}

void PlayState::updateWindowTitle(Game& game)
{
	game.getWindow().setTitle("Metal Slug OOP - Score: " + std::to_string(scoreManager.getScore()));
}

void PlayState::handleInput(Game& game)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		game.changeState(new ModeSelectState());
		return;
	}

	if (waitingForContinue && sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		advanceLevel(game);
	}
}

void PlayState::update(Game& game, float deltaTime)
{
	if (!initialized)
	{
		initialize(game);
	}

	DeveloperMode::update(deltaTime);

	if (gameOver)
	{
		updateCamera(game);
		updateHud(game);
		return;
	}

	game.getLevelManager().update(deltaTime);
	if (player != 0 && player->isActive())
	{
		player->handleWeaponInput(game.getEntityManager(), deltaTime);
	}
	game.getEntityManager().updateAll(deltaTime);

	if (player != 0 && player->isDead() && !DeveloperMode::isEnabled())
	{
		showGameOver();
		updateCamera(game);
		updateHud(game);
		return;
	}

	int gainedScore = game.getEntityManager().collectPendingScore();
	if (gainedScore > 0)
	{
		scoreManager.add(gainedScore);
		if (mode == PLAY_MODE_CAMPAIGN)
		{
			campaignKills += 1;
		}
		updateWindowTitle(game);
	}

	updateCampaignSpawning(game, deltaTime);

	if (!waitingForContinue && mode != PLAY_MODE_CAMPAIGN && game.getEntityManager().countActiveEnemies() == 0)
	{
		Level* level = game.getLevelManager().getCurrentLevel();
		if (level != 0 && player != 0 && player->getX() >= level->getRightBoundary() - 220.0f)
		{
			completeLevel(game);
		}
	}

	updateCamera(game);
	updateHud(game);
}

void PlayState::draw(Game& game, sf::RenderWindow& window)
{
	window.setView(worldView);
	game.getLevelManager().draw(window);
	game.getEntityManager().drawAll(window);

	window.setView(window.getDefaultView());
	window.draw(hudText);
	if (waitingForContinue || gameOver)
	{
		window.draw(centerText);
	}
}

void PlayState::updateCamera(Game& game)
{
	Level* level = game.getLevelManager().getCurrentLevel();
	if (level == 0 || player == 0)
	{
		worldView = game.getWindow().getDefaultView();
		return;
	}

	float centerX = player->getCenterX();
	float halfWidth = static_cast<float>(Constants::SCREEN_WIDTH) / 2.0f;
	if (centerX < halfWidth)
	{
		centerX = halfWidth;
	}
	if (centerX > level->getWorldWidth() - halfWidth)
	{
		centerX = level->getWorldWidth() - halfWidth;
	}

	worldView.setSize(static_cast<float>(Constants::SCREEN_WIDTH), static_cast<float>(Constants::SCREEN_HEIGHT));
	worldView.setCenter(centerX, static_cast<float>(Constants::SCREEN_HEIGHT) / 2.0f);
}

void PlayState::updateHud(Game& game)
{
	(void)game;
	std::string modeName = mode == PLAY_MODE_CAMPAIGN ? "Campaign" : "Survival";
	std::string text = modeName + " L" + std::to_string(currentLevel);
	text += "  Score " + std::to_string(scoreManager.getScore());
	if (player != 0)
	{
		text += "  " + std::string(player->getCharacterName());
		text += " HP " + std::to_string(player->getHealth());
		text += " Lives " + std::to_string(player->getLives());
		text += " G " + std::to_string(player->getGrenades());
		text += " R " + std::to_string(player->getRockets());
	}
	if (mode == PLAY_MODE_CAMPAIGN)
	{
		text += "  Kills " + std::to_string(campaignKills);
		text += "  Enemies " + std::to_string(game.getEntityManager().countActiveEnemies()) + "/10";
	}
	if (DeveloperMode::isEnabled())
	{
		text += "  DEV MODE";
	}
	text += "\nJ Shoot  K Knife  G Grenade  H Rocket  Z Switch  F1x3 Dev";
	hudText.setString(text);
}

void PlayState::completeLevel(Game& game)
{
	if (mode == PLAY_MODE_CAMPAIGN)
	{
		return;
	}
	else if (currentLevel >= 4)
	{
		scoreManager.addSurvivalClear();
		centerText.setString("SURVIVAL CLEAR\nPress Enter for menu");
	}
	else
	{
		scoreManager.addSurvivalClear();
		centerText.setString("LEVEL CLEAR\nPress Enter");
	}

	waitingForContinue = true;
	updateWindowTitle(game);
}

void PlayState::showGameOver()
{
	gameOver = true;
	waitingForContinue = false;
	centerText.setString("GAME OVER\nPress Escape to return to menu");
	centerText.setPosition(360.0f, 360.0f);
}

void PlayState::advanceLevel(Game& game)
{
	if (mode == PLAY_MODE_CAMPAIGN || currentLevel >= 4)
	{
		game.changeState(new ModeSelectState());
		return;
	}

	currentLevel += 1;
	loadCurrentLevel(game);
}
