#pragma once
#include <SFML/Audio.hpp>
#include "GameState.h"
using namespace sf;

class ModeSelectState : public GameState
{

private:
	int selectedOption;
	bool showingCampaignMessage;

	Font font;
	Texture backgroundTexture;
	Sprite backgroundSprite;
	Text title;
	Text survivalOption;
	Text campaignOption;
	Text bossLevelOption;
	Text instructions;
	Text message;

	Music menuMusic;
	SoundBuffer moveSoundBuffer;
	Sound moveSound;


	void playMoveSound();
	void setupScreen();
	void refreshOptions();
	void startSelectedMode(Game& game);

public:
	ModeSelectState();

	void handleInput(Game& game);
	void update(Game& game, float deltaTime);
	void draw(Game& game, RenderWindow& window);
};
