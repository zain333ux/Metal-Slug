#include "PlayState.h"

#include "Constants.h"
#include "Enemy.h"
#include "EnemyFactory.h"
#include "EnemyVehicleFactory.h"
#include "Game.h"
#include "Level.h"
#include "LevelProfile.h"
#include "PlayerSoldier.h"
#include "Prisoner.h"
#include "DeveloperMode.h"
#include "ModeSelectState.h"
#include "Vehicle.h"
#include "SlugFlyer.h"
#include "Submarine.h"

#include <string>


using namespace std;
using namespace sf;

namespace
{
	bool findWaterVehicleSpawn(Level* level, float preferredX, float& spawnX, float& spawnY)
	{
		if (level == 0)
		{
			return false;
		}

		const float worldWidth = level->getWorldWidth();
		const float step = 160;

		for (int offset = 0; offset < 40; offset += 1)
		{
			for (int side = 0; side < 2; side += 1)
			{
				float testX = preferredX + step * static_cast<float>(offset);
				if (side == 1)
				{
					testX = preferredX - step * static_cast<float>(offset);
				}

				if (testX < 120)
				{
					testX = 120;
				}
				if (testX > worldWidth - 220)
				{
					testX = worldWidth - 220;
				}

				float waterY = level->getWaterSurfaceYAt(testX);
				if (waterY <= level->getWorldHeight() - 90)
				{
					spawnX = testX;
					spawnY = waterY + 50;
					return true;
				}
			}
		}

		return false;
	}
}

PlayState::PlayState(PlayMode newMode, int newCampaignProfileOption, int startLevel)
{
	initialized = false;
	levelComplete = false;
	waitingForContinue = false;
	gameOver = false;
	mode = newMode;
	currentLevel = startLevel;
	campaignProfileOption = newCampaignProfileOption;
	campaignSpawnX = 1200;
	campaignSpawnTimer = 0;
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
	hudText.setFillColor(Color::White);
	hudText.setPosition(18, 18);

	centerText.setFont(hudFont);
	centerText.setCharacterSize(28);
	centerText.setFillColor(Color(255, 240, 130));
	centerText.setPosition(320, 360);
}

void PlayState::loadCurrentLevel(Game& game)
{
	game.getEntityManager().clear();
	player = 0;
	vehicle = 0;
	previousVehicleKey = false;
	bossLevelManager.reset();

	player = new PlayerSoldier();
	game.getEntityManager().addEntity(player);

	if (mode == PLAY_MODE_CAMPAIGN)
	{
		LevelProfile* profile = LevelProfileFactory::createProfile(campaignProfileOption);
		game.getLevelManager().loadLevel(new GameLevel(1, profile->getWorldWidth(), true));
		delete profile;
		spawnCampaignWave(game);
	}
	else if (currentLevel == 4)
	{
		game.getLevelManager().loadLevel(new GameLevel(4, Constants::BOSS_WORLD_WIDTH, false));
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
		game.getLevelManager().loadLevel(new GameLevel(currentLevel, width, false));
		spawnSurvivalWave(game);
	}

	if (mode == PLAY_MODE_CAMPAIGN || currentLevel == 1)
	{
		game.getAudioManager().playMusic(MUSIC_LEVEL1);
	}
	else if (currentLevel == 2)
	{
		game.getAudioManager().playMusic(MUSIC_LEVEL2);
	}
	else if (currentLevel == 3)
	{
		game.getAudioManager().playMusic(MUSIC_LEVEL3);
	}
	else if (currentLevel == 4)
	{
		game.getAudioManager().playMusic(MUSIC_BOSS);
	}

	Level* loadedLevel = game.getLevelManager().getCurrentLevel();
	if (loadedLevel != 0)
	{
		game.getEntityManager().setActiveLevel(loadedLevel);
		player->setMovementMaxX(loadedLevel->getWorldWidth());
		player->setPosition(120, loadedLevel->getGroundYAt(120) - player->getHeight());
	}

	spawnVehicle(game);
	if (mode != PLAY_MODE_CAMPAIGN && currentLevel == 4)
	{
		bossLevelManager.startBossLevel(game.getEntityManager(), player, game.getLevelManager());
	}
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
	bool flyingEnemy = kind == ENEMY_MARTIAN || kind == ENEMY_BOSS_2;
	if (level != 0 && !flyingEnemy)
	{
		enemy->setPosition(x, level->getGroundYAt(x) - enemy->getHeight());
	}
	game.getEntityManager().addEntity(enemy);
}

void PlayState::spawnSurvivalPrisoner(Game& game)
{
	if (mode != PLAY_MODE_SURVIVAL || currentLevel < 1 || currentLevel > 3)
	{
		return;
	}

	Level* level = game.getLevelManager().getCurrentLevel();
	if (level == 0)
	{
		return;
	}

	float prisonerX = 2480;
	if (currentLevel == 2)
	{
		prisonerX = 3180;
	}
	else if (currentLevel == 3)
	{
		prisonerX = 3820;
	}

	float groundY = level->getGroundYAt(prisonerX);
	Prisoner* prisoner = new Prisoner(prisonerX, groundY);
	prisoner->setActiveLevel(level);
	game.getEntityManager().addEntity(prisoner);
}

void PlayState::spawnSurvivalWave(Game& game)
{
	spawnSurvivalPrisoner(game);

	float base = static_cast<float>(currentLevel - 1) * 160;
	if (currentLevel == 1)
	{
		spawnEnemy(game, ENEMY_REBEL, 520, 500);
		spawnEnemy(game, ENEMY_SHIELDED, 840, 500);
		spawnEnemy(game, ENEMY_BAZOOKA, 1120, 500);
		spawnEnemy(game, ENEMY_GRENADE, 1360, 500);
		spawnEnemy(game, ENEMY_BAZOOKA, 1740, 500);
		spawnEnemy(game, ENEMY_GRENADE, 1900, 519);
		spawnEnemy(game, ENEMY_ZOMBIE, 2040, 500);
		spawnEnemy(game, ENEMY_MUMMY, 2160, 500);
		spawnEnemy(game, ENEMY_SHIELDED, 2280, 500);
		spawnEnemy(game, ENEMY_GRENADE, 2760, 500);
		spawnEnemy(game, ENEMY_MARTIAN, 3260, 360);
		return;
	}
	spawnEnemy(game, ENEMY_REBEL, 520 + base, 500);
	spawnEnemy(game, ENEMY_REBEL, 1160 + base, 500);
	spawnEnemy(game, ENEMY_REBEL, 1500 + base, 500);
	spawnEnemy(game, ENEMY_SHIELDED, 2180 + base, 500);
	spawnEnemy(game, ENEMY_BAZOOKA, 2500 + base, 500);
	spawnEnemy(game, ENEMY_GRENADE, 2800 + base, 500);
	spawnEnemy(game, ENEMY_ZOMBIE, 2920 + base, 500);
	spawnEnemy(game, ENEMY_MUMMY, 3020 + base, 500);
	spawnEnemy(game, ENEMY_SHIELDED, 3080 + base, 500);
	spawnEnemy(game, ENEMY_BAZOOKA, 3320 + base, 500);
	spawnEnemy(game, ENEMY_MARTIAN, 3480 + base, 360);
	if (currentLevel >= 2)
	{
		spawnEnemy(game, ENEMY_REBEL, 3600, 500);
		spawnEnemy(game, ENEMY_REBEL, 3690, 500);
		spawnEnemy(game, ENEMY_GRENADE, 3960, 500);
	}
	if (currentLevel >= 3)
	{
		spawnEnemy(game, ENEMY_SHIELDED, 4380, 500);
		spawnEnemy(game, ENEMY_MARTIAN, 4740, 360);
	}

	Level* level = game.getLevelManager().getCurrentLevel();
	EnemyVehicleFactory::spawnEnemyVehiclesForSurvivalLevel(currentLevel, level, game.getEntityManager(), player);
}

void PlayState::spawnCampaignWave(Game& game)
{
	int activeEnemies = game.getEntityManager().countActiveEnemies();
	const int enemyCap = 10;

	for (int i = 0; i < 3 && activeEnemies < enemyCap; i += 1)
	{
		float x = campaignSpawnX + static_cast<float>(i) * 480;

		if (campaignKills > 18 && i == 2)
		{
			spawnEnemy(game, ENEMY_MARTIAN, x, 360);
		}
		else if (campaignKills > 10 && i == 1)
		{
			spawnEnemy(game, ENEMY_GRENADE, x + 320, 500);
		}
		else if (campaignKills > 7 && i == 2)
		{
			spawnEnemy(game, ENEMY_MUMMY, x + 240, 500);
		}
		else if (campaignKills > 3 && i == 0)
		{
			spawnEnemy(game, ENEMY_ZOMBIE, x, 500);
		}
		else if (campaignKills > 5 && i == 1)
		{
			spawnEnemy(game, ENEMY_SHIELDED, x + 160, 500);
		}
		else
		{
			spawnEnemy(game, ENEMY_REBEL, x, 500);
			if (activeEnemies + 1 < enemyCap)
			{
				spawnEnemy(game, ENEMY_REBEL, x + 85, 500);
				activeEnemies += 1;
			}
		}

		activeEnemies += 1;
	}

	campaignSpawnX += 1450;
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

	game.getEntityManager().removeEnemiesBehind(player->getX() - 1300);

	if (campaignSpawnTimer > 0)
	{
		campaignSpawnTimer -= deltaTime;
	}
	if (campaignVehicleSpawnTimer > 0)
	{
		campaignVehicleSpawnTimer -= deltaTime;
	}

	if (campaignSpawnTimer <= 0 && game.getEntityManager().countActiveEnemies() < 10)
	{
		if (campaignSpawnX < player->getX() + 900)
		{
			campaignSpawnX = player->getX() + 900;
		}
		spawnCampaignWave(game);
		game.getEntityManager().setActiveLevel(level);
		campaignSpawnTimer = 2;
	}

	if (campaignVehicleSpawnTimer <= 0)
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

void PlayState::spawnVehicle(Game& game)
{
	if (mode != PLAY_MODE_CAMPAIGN && currentLevel == 4)
	{
		return;
	}

	Level* level = game.getLevelManager().getCurrentLevel();
	bool aquaticVehicleLevel = currentLevel == 3 || (mode == PLAY_MODE_CAMPAIGN && campaignProfileOption == 2);
	float vehicleX = 330;
	float vehicleY = static_cast<float>(Constants::GROUND_Y) - 96;

	if (level != 0)
	{
		vehicleY = level->getMainGroundYAt(vehicleX) - 96;
	}

	if (!aquaticVehicleLevel)
	{
		vehicle = new MetalSlug(vehicleX, vehicleY);
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
		float flyerY = level->getMainGroundYAt(flyerX) - 220;
		if (flyerY < 120)
		{
			flyerY = 120;
		}
		SlugFlyer* flyer = new SlugFlyer(flyerX, flyerY);
		flyer->setMovementMaxX(level->getWorldWidth());
		flyer->setActiveLevel(level);
		game.getEntityManager().addEntity(flyer);
	}

	// Aquatic levels use the Slug Mariner. Search for real water so it never spawns below the map.
	if (aquaticVehicleLevel)
	{
		float subX = 1800;
		float subY = 600;
		if (level != 0 && !findWaterVehicleSpawn(level, level->getWorldWidth() * 0.30f, subX, subY))
		{
			subX = level->getWorldWidth() * 0.30f;
			subY = level->getMainGroundYAt(subX) - 90;
		}
		SlugMariner* sub = new SlugMariner(subX, subY);
		sub->setMovementMaxX(level ? level->getWorldWidth() : 20000);
		sub->setActiveLevel(level);
		game.getEntityManager().addEntity(sub);
		vehicle = sub;
	}
}

void PlayState::handleVehicleInteraction(Game& game)
{
	if (player == 0 || !player->isActive() || player->isDead() || !player->canUseVehicle())
	{
		previousVehicleKey = Keyboard::isKeyPressed(Keyboard::E);
		return;
	}

	vehicle = game.getEntityManager().getClosestVehicle(player->getCenterX(), player->getCenterY());

	bool vehicleKey = Keyboard::isKeyPressed(Keyboard::E);
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
		player->setVelocity(0, 0);
		player->setPosition(vehicle->getSeatX(), vehicle->getSeatY());
	}
}

void PlayState::updateWindowTitle(Game& game)
{
	game.getWindow().setTitle("Metal Slug OOP - Score: " + to_string(scoreManager.getScore()));
}

void PlayState::handleInput(Game& game)
{
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		game.changeState(new ModeSelectState());
		return;
	}

	if (waitingForContinue && Keyboard::isKeyPressed(Keyboard::Return))
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
	if (mode != PLAY_MODE_CAMPAIGN && currentLevel == 4)
	{
		bossLevelManager.update(deltaTime, game.getEntityManager(), player, game.getLevelManager());
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

	if (!waitingForContinue && mode != PLAY_MODE_CAMPAIGN && currentLevel == 4 && bossLevelManager.isCompleted())
	{
		completeLevel(game);
	}
	else if (!waitingForContinue && mode != PLAY_MODE_CAMPAIGN && currentLevel != 4)
	{
		Level* level = game.getLevelManager().getCurrentLevel();
		if (level != 0 && player != 0 &&
			player->getX() >= level->getRightBoundary() - 220 &&
			game.getEntityManager().countActiveEnemies() == 0)
		{
			completeLevel(game);
		}
	}

	updateCamera(game);
	updateHud(game);
}

void PlayState::draw(Game& game, RenderWindow& window)
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
	float halfWidth = viewWidth / 2;
	float halfHeight = viewHeight / 2;
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
	string modeName = mode == PLAY_MODE_CAMPAIGN ? "Campaign" : "Survival";
	string text = modeName + " L" + to_string(currentLevel);
	text += "  Score " + to_string(scoreManager.getScore());
	if (player != 0)
	{
		text += "  " + string(player->getCharacterName());
		text += " HP " + to_string(player->getHealth());
		text += " Lives " + to_string(player->getLives());
		text += " G " + to_string(player->getGrenades());
		text += " R " + to_string(player->getRockets());
		text += " HMG " + to_string(player->getHmgBullets());
	}
	if (vehicle != 0)
	{
		if (vehicle->isOccupied())
		{
			text += "  Vehicle HP " + to_string(vehicle->getHealth());
			text += "/" + to_string(vehicle->getMaxHealth());
		}
		else if (player != 0 && vehicle->canMount(*player))
		{
			text += "  Press E Vehicle";
		}
	}
	if (mode == PLAY_MODE_CAMPAIGN)
	{
		text += "  Kills " + to_string(campaignKills);
		text += "  Enemies " + to_string(game.getEntityManager().countActiveEnemies()) + "/10";
		text += "  VT " + to_string(game.getEntityManager().getDestroyedFlyingTaraCount()) + "/3";
		text += "  B " + to_string(game.getEntityManager().getDestroyedBradleyCount()) + "/3";
		text += "  S " + to_string(game.getEntityManager().getDestroyedEnemySubCount()) + "/3";
	}
	else
	{
		int enemiesLeft = game.getEntityManager().countActiveEnemies();
		text += "  Enemies " + to_string(enemiesLeft);
		Level* level = game.getLevelManager().getCurrentLevel();
		if (level != 0 && player != 0 && player->getX() >= level->getRightBoundary() - 220 && enemiesLeft > 0)
		{
			text += "  Clear Enemies";
		}
	}
	if (mode != PLAY_MODE_CAMPAIGN && currentLevel == 4)
	{
		bossLevelManager.appendHudText(text);
	}
	if (DeveloperMode::isEnabled())
	{
		text += "  DEV MODE";
	}
	text += "\nJ Shoot  K Knife  G Grenade  Z Switch  E Vehicle  F1x3 Dev";
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
	centerText.setPosition(360, 360);
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
