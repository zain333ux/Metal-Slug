#pragma once
#include "GameState.h"

class ModeSelectState : public GameState
{

private:
	int selectedOption;
	bool showingCampaignMessage;
	bool previousUpKey;
	bool previousDownKey;
	bool previousSelectKey;

	sf::Font font;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Text title;
	sf::Text survivalOption;
	sf::Text campaignOption;
	sf::Text bossLevelOption;
	sf::Text instructions;
	sf::Text message;

	void setupScreen();
	void refreshOptions();
	void startSelectedMode(Game& game);

public:
	ModeSelectState();

	void handleInput(Game& game);
	void update(Game& game, float deltaTime);
	void draw(Game& game, sf::RenderWindow& window);
};
