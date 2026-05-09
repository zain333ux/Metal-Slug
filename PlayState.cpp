#include "PlayState.h"

#include "Collectible.h"
#include "Constants.h"
#include "Enemy.h"
#include "EnemyFactory.h"
#include "EnemyVehicleFactory.h"
#include "Game.h"
#include "Level.h"
#include "LevelProfile.h"
#include "PlayerSoldier.h"
#include "DeveloperMode.h"
#include "ModeSelectState.h"
#include "Vehicle.h"
#include "SlugFlyer.h"
#include "Submarine.h"

#include <string>

namespace
{
	bool findWaterVehicleSpawn(Level* level, float preferredX, float& spawnX, float& spawnY)
	{
		if (level == 0)
		{
			return false;
		}

		const float worldWidth = level->getWorldWidth();
		const float step = 160.0f;

		for (int offset = 0; offset < 40; offset += 1)
		{
			for (int side = 0; side < 2; side += 1)
			{
				float testX = preferredX + step * static_cast<float>(offset);
				if (side == 1)
				{
					testX = preferredX - step * static_cast<float>(offset);
				}

				if (testX < 120.0f)
				{
					testX = 120.0f;
				}
				if (testX > worldWidth - 220.0f)
				{
					testX = worldWidth - 220.0f;
				}

				float waterY = level->getWaterSurfaceYAt(testX);
				if (waterY <= level->getWorldHeight() - 90.0f)
				{
					spawnX = testX;
					spawnY = waterY + 50.0f;
					return true;
				}
			}
		}

		return false;
	}
}

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
	campaignVehicleSpawnTimer = 2.5f;
	campaignKills = 0;
	player = 0;
	vehicle = 0;
	previousVehicleKey = false;
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
	player = 0;
	vehicle = 0;
	previousVehicleKey = false;

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
		player->setPosition(120.0f, loadedLevel->getGroundYAt(120.0f) - player->getHeight());
	}

	spawnVehicle(game);
	spawnPickups(game);
	campaignVehicleSpawnTimer = 2.5f;
	levelComplete = false;
	waitingForContinue = false;
	gameOver = false;
}

void PlayState::spawnEnemy(Game& game, EnemyKind kind, float x, float y)
{
	Enemy* enemy = EnemyFactory::createEnemy(kind, x, y, player);
	bool bossEnemy = kind == ENEMY_BOSS_1 || kind == ENEMY_BOSS_2 || kind == ENEMY_BOSS_3 || kind == ENEMY_ULTIMATE_BOSS;
	if (!bossEnemy && !enemy->hasSpriteVisual())
	{
		delete enemy;
		return;
	}

	Level* level = game.getLevelManager().getCurrentLevel();
	bool flyingEnemy = kind == ENEMY_BOSS_2;
	if (level != 0 && !flyingEnemy)
	{
		enemy->setPosition(x, level->getGroundYAt(x) - enemy->getHeight());
	}
	game.getEntityManager().addEntity(enemy);
}

void PlayState::spawnEnemyAt(Game& game, EnemyKind kind, float x, float y)
{
	Enemy* enemy = EnemyFactory::createEnemy(kind, x, y, player);
	bool bossEnemy = kind == ENEMY_BOSS_1 || kind == ENEMY_BOSS_2 || kind == ENEMY_BOSS_3 || kind == ENEMY_ULTIMATE_BOSS;
	if (!bossEnemy && !enemy->hasSpriteVisual())
	{
		delete enemy;
		return;
	}

	game.getEntityManager().addEntity(enemy);
}

void PlayState::spawnSurvivalWave(Game& game)
{
	float base = static_cast<float>(currentLevel - 1) * 160.0f;
	if (currentLevel == 1)
	{
		spawnEnemy(game, ENEMY_REBEL, 520.0f, 500.0f);
		spawnEnemy(game, ENEMY_SHIELDED, 840.0f, 500.0f);
		spawnEnemy(game, ENEMY_BAZOOKA, 1120.0f, 500.0f);
		spawnEnemy(game, ENEMY_GRENADE, 1360.0f, 500.0f);
		spawnEnemy(game, ENEMY_BAZOOKA, 1740.0f, 500.0f);
		spawnEnemy(game, ENEMY_GRENADE, 1900.0f, 519.0f);
		spawnEnemy(game, ENEMY_SHIELDED, 2280.0f, 500.0f);
		spawnEnemy(game, ENEMY_GRENADE, 2760.0f, 500.0f);
		spawnEnemy(game, ENEMY_MARTIAN, 3260.0f, 360.0f);
		return;
	}
	spawnEnemy(game, ENEMY_REBEL, 520.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_REBEL, 1160.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_REBEL, 1500.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_SHIELDED, 2180.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_BAZOOKA, 2500.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_GRENADE, 2800.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_SHIELDED, 3080.0f + base, 500.0f);
	spawnEnemy(game, ENEMY_BAZOOKA, 3320.0f + base, 500.0f);
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

	Level* level = game.getLevelManager().getCurrentLevel();
	EnemyVehicleFactory::spawnEnemyVehiclesForSurvivalLevel(currentLevel, level, game.getEntityManager(), player);
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
		if (vehicle != 0)
		{
			vehicle->setMovementMaxX(level->getWorldWidth());
		}
	}

	game.getEntityManager().removeEnemiesBehind(player->getX() - 1300.0f);

	if (campaignSpawnTimer > 0.0f)
	{
		campaignSpawnTimer -= deltaTime;
	}
	if (campaignVehicleSpawnTimer > 0.0f)
	{
		campaignVehicleSpawnTimer -= deltaTime;
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

	if (campaignVehicleSpawnTimer <= 0.0f)
	{
		EnemyVehicleFactory::spawnEnemyVehiclesForCampaign(
			level,
			game.getEntityManager(),
			player,
			game.getEntityManager().getDestroyedFlyingTaraCount(),
			game.getEntityManager().getDestroyedBradleyCount(),
			game.getEntityManager().getDestroyedEnemySubCount());
		campaignVehicleSpawnTimer = 7.5f;
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

void PlayState::spawnVehicle(Game& game)
{
	Level* level = game.getLevelManager().getCurrentLevel();
	bool aquaticVehicleLevel = currentLevel == 3 || (mode == PLAY_MODE_CAMPAIGN && campaignProfileOption == 2);
	float vehicleX = 330.0f;
	float vehicleY = static_cast<float>(Constants::GROUND_Y) - 96.0f;

	if (level != 0)
	{
		vehicleY = level->getMainGroundYAt(vehicleX) - 96.0f;
	}

	if (!aquaticVehicleLevel)
	{
		vehicle = new Vehicle(vehicleX, vehicleY);
		if (level != 0)
		{
			vehicle->setMovementMaxX(level->getWorldWidth());
		}
		game.getEntityManager().addEntity(vehicle);
	}

	// Spawn Slug Flyer in higher terrain regions for aerial biome traversal.
	if (level != 0 && !aquaticVehicleLevel && (mode == PLAY_MODE_CAMPAIGN || currentLevel <= 3))
	{
		float flyerX = level->getWorldWidth() * 0.28f;
		float flyerY = level->getMainGroundYAt(flyerX) - 220.0f;
		if (flyerY < 120.0f)
		{
			flyerY = 120.0f;
		}
		SlugFlyer* flyer = new SlugFlyer(flyerX, flyerY);
		flyer->setMovementMaxX(level->getWorldWidth());
		flyer->setActiveLevel(level);
		game.getEntityManager().addEntity(flyer);
	}

	// Aquatic levels use the Slug Mariner. Search for real water so it never spawns below the map.
	if (aquaticVehicleLevel)
	{
		float subX = 1800.0f;
		float subY = 600.0f;
		if (level != 0 && !findWaterVehicleSpawn(level, level->getWorldWidth() * 0.30f, subX, subY))
		{
			subX = level->getWorldWidth() * 0.30f;
			subY = level->getMainGroundYAt(subX) - 90.0f;
		}
		SlugMariner* sub = new SlugMariner(subX, subY);
		sub->setMovementMaxX(level ? level->getWorldWidth() : 20000.0f);
		sub->setActiveLevel(level);
		game.getEntityManager().addEntity(sub);
		vehicle = sub;
	}
}

void PlayState::handleVehicleInteraction(Game& game)
{
	if (player == 0 || !player->isActive() || player->isDead())
	{
		previousVehicleKey = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		return;
	}

	vehicle = game.getEntityManager().getClosestVehicle(player->getCenterX(), player->getCenterY());

	bool vehicleKey = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
	if (vehicleKey && !previousVehicleKey)
	{
		if (player->isRidingVehicle())
		{
			if (vehicle != 0)
			{
				vehicle->dismount(*player);
			}
			else
			{
				player->setRidingVehicle(false);
			}
		}
		else if (vehicle != 0 && vehicle->canMount(*player))
		{
			vehicle->mount(*player);
		}
	}

	previousVehicleKey = vehicleKey;
}

void PlayState::syncPlayerWithVehicle()
{
	if (player == 0 || vehicle == 0)
	{
		return;
	}

	if (player->isRidingVehicle() && vehicle->isOccupied())
	{
		player->setVelocity(0.0f, 0.0f);
		player->setPosition(vehicle->getSeatX(), vehicle->getSeatY());
	}
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

	vehicle = game.getEntityManager().getVehicle();
	handleVehicleInteraction(game);
	syncPlayerWithVehicle();

	game.getLevelManager().update(deltaTime);
	if (player != 0 && player->isActive())
	{
		if (player->isRidingVehicle() && vehicle != 0)
		{
			vehicle->handleWeaponInput(game.getEntityManager(), deltaTime);
		}
		else
		{
			player->handleWeaponInput(game.getEntityManager(), deltaTime);
		}
	}
	game.getEntityManager().updateAll(deltaTime);
	vehicle = game.getEntityManager().getVehicle();
	syncPlayerWithVehicle();

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

	if (mode == PLAY_MODE_CAMPAIGN && !waitingForContinue)
	{
		bool vehicleQuotaMet =
			game.getEntityManager().getDestroyedFlyingTaraCount() >= 3 &&
			game.getEntityManager().getDestroyedBradleyCount() >= 3 &&
			game.getEntityManager().getDestroyedEnemySubCount() >= 3;
		if (vehicleQuotaMet)
		{
			waitingForContinue = true;
			centerText.setString("CAMPAIGN VEHICLE QUOTA CLEAR\nPress Enter");
		}
	}

	if (!waitingForContinue && mode != PLAY_MODE_CAMPAIGN)
	{
		Level* level = game.getLevelManager().getCurrentLevel();
		if (level != 0 && player != 0 &&
			player->getX() >= level->getRightBoundary() - 220.0f &&
			game.getEntityManager().countActiveEnemies() == 0)
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
	float centerY = player->getCenterY();
	float cameraZoom = 1.45f;
	float viewWidth = static_cast<float>(Constants::SCREEN_WIDTH) * cameraZoom;
	float viewHeight = static_cast<float>(Constants::SCREEN_HEIGHT) * cameraZoom;
	float halfWidth = viewWidth / 2.0f;
	float halfHeight = viewHeight / 2.0f;
	float topBias = (viewHeight - static_cast<float>(Constants::SCREEN_HEIGHT)) * 0.5f;
	centerY -= topBias;
	if (centerX < halfWidth)
	{
		centerX = halfWidth;
	}
	if (centerX > level->getWorldWidth() - halfWidth)
	{
		centerX = level->getWorldWidth() - halfWidth;
	}
	if (centerY < halfHeight)
	{
		centerY = halfHeight;
	}
	if (centerY > level->getWorldHeight() - halfHeight)
	{
		centerY = level->getWorldHeight() - halfHeight;
	}

	worldView.setSize(viewWidth, viewHeight);
	worldView.setCenter(centerX, centerY);
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
	if (vehicle != 0)
	{
		if (vehicle->isOccupied())
		{
			text += "  Vehicle HP " + std::to_string(vehicle->getHealth());
			text += "/" + std::to_string(vehicle->getMaxHealth());
		}
		else if (player != 0 && vehicle->canMount(*player))
		{
			text += "  Press E Vehicle";
		}
	}
	if (mode == PLAY_MODE_CAMPAIGN)
	{
		text += "  Kills " + std::to_string(campaignKills);
		text += "  Enemies " + std::to_string(game.getEntityManager().countActiveEnemies()) + "/10";
		text += "  VT " + std::to_string(game.getEntityManager().getDestroyedFlyingTaraCount()) + "/3";
		text += "  B " + std::to_string(game.getEntityManager().getDestroyedBradleyCount()) + "/3";
		text += "  S " + std::to_string(game.getEntityManager().getDestroyedEnemySubCount()) + "/3";
	}
	else
	{
		int enemiesLeft = game.getEntityManager().countActiveEnemies();
		text += "  Enemies " + std::to_string(enemiesLeft);
		Level* level = game.getLevelManager().getCurrentLevel();
		if (level != 0 && player != 0 && player->getX() >= level->getRightBoundary() - 220.0f && enemiesLeft > 0)
		{
			text += "  Clear Enemies";
		}
	}
	if (DeveloperMode::isEnabled())
	{
		text += "  DEV MODE";
	}
	text += "\nJ Shoot  K Knife  G Grenade  H Rocket  Z Switch  E Vehicle  F1x3 Dev";
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
