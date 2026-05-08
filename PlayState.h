#pragma once

#include "GameState.h"
#include "EnemyFactory.h"
#include "ScoreManager.h"

class PlayerSoldier;
class Vehicle;

enum PlayMode
{
	PLAY_MODE_SURVIVAL,
	PLAY_MODE_CAMPAIGN
};

class PlayState : public GameState
{
private:
	bool initialized;
	bool levelComplete;
	bool waitingForContinue;
	bool gameOver;
	PlayMode mode;
	int currentLevel;
	int campaignProfileOption;
	float campaignSpawnX;
	float campaignSpawnTimer;
	int campaignKills;
	ScoreManager scoreManager;
	PlayerSoldier* player;
	Vehicle* vehicle;
	bool previousVehicleKey;
	sf::Font hudFont;
	sf::Text hudText;
	sf::Text centerText;
	sf::View worldView;

	void initialize(Game& game);
	void configureHud();
	void loadCurrentLevel(Game& game);
	void spawnEnemy(Game& game, EnemyKind kind, float x, float y);
	void spawnEnemyAt(Game& game, EnemyKind kind, float x, float y);
	void spawnSurvivalWave(Game& game);
	void spawnBossWave(Game& game);
	void spawnCampaignWave(Game& game);
	void updateCampaignSpawning(Game& game, float deltaTime);
	void spawnPickups(Game& game);
	void spawnVehicle(Game& game);
	void handleVehicleInteraction(Game& game);
	void syncPlayerWithVehicle();
	void updateCamera(Game& game);
	void updateHud(Game& game);
	void completeLevel(Game& game);
	void showGameOver();
	void advanceLevel(Game& game);
	void updateWindowTitle(Game& game);

public:
	PlayState(PlayMode newMode = PLAY_MODE_SURVIVAL, int newCampaignProfileOption = 0);

	void handleInput(Game& game);
	void update(Game& game, float deltaTime);
	void draw(Game& game, sf::RenderWindow& window);
};
