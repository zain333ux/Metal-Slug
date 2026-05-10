#include <SFML/Graphics.hpp>
#include "ModeSelectState.h"
#include "Game.h"
#include "Level.h"
#include "PlayState.h"


using namespace std;
using namespace sf;

ModeSelectState::ModeSelectState()
{
	selectedOption = 0;
	showingCampaignMessage = false;
	previousUpKey = false;
	previousDownKey = false;
	previousSelectKey = false;

	font.loadFromFile("25I-0881_25I-0807_Assets/Fonts/PressStart2P.ttf");

	backgroundTexture.loadFromFile("25I-0881_25I-0807_Assets/Sprites/Clean/Game_Select_BG.jpg");
	backgroundSprite.setTexture(backgroundTexture);

	setupScreen();
	refreshOptions();
}

void ModeSelectState::setupScreen()
{
	title.setFont(font);
	title.setString("METAL SLUG");
	title.setCharacterSize(70);
	title.setFillColor(Color(245,245,245));
	title.setPosition(480, 180);

	survivalOption.setFont(font);
	survivalOption.setCharacterSize(36);
	survivalOption.setFillColor(Color(80, 220, 255));
	survivalOption.setPosition(540, 360);

	campaignOption.setFont(font);
	campaignOption.setCharacterSize(36);
	campaignOption.setFillColor(Color(230, 230, 230));
	campaignOption.setPosition(540, 450);

	bossLevelOption.setFont(font);
	bossLevelOption.setCharacterSize(36);
	bossLevelOption.setFillColor(Color(230, 230, 230));
	bossLevelOption.setPosition(540, 540);

	instructions.setFont(font);
	instructions.setString("Use Up and Down. Press Enter.");
	instructions.setCharacterSize(24);
	instructions.setFillColor(Color(210, 220, 230));
	instructions.setPosition(540, 650);

	message.setFont(font);
	message.setString("Campaign profile: generated plains.");
	message.setCharacterSize(24);
	message.setFillColor(Color(255, 120, 120));
	message.setPosition(540, 700);
}

void ModeSelectState::refreshOptions()
{
	if (selectedOption == 0)
	{
		survivalOption.setString("> Survival Mode");
		campaignOption.setString("  Campaign Mode");
		bossLevelOption.setString("  Boss Level ");

		survivalOption.setFillColor(Color(80, 220, 255));
		campaignOption.setFillColor(Color(230, 230, 230));
		bossLevelOption.setFillColor(Color(230, 230, 230));
	}
	else if (selectedOption == 1)
	{
		survivalOption.setString("  Survival Mode");
		campaignOption.setString("> Campaign Mode");
		bossLevelOption.setString("  Boss Level ");

		survivalOption.setFillColor(Color(230, 230, 230));
		campaignOption.setFillColor(Color(80, 220, 255));
		bossLevelOption.setFillColor(Color(230, 230, 230));
	}
	else
	{
		survivalOption.setString("  Survival Mode");
		campaignOption.setString("  Campaign Mode");
		bossLevelOption.setString("> Boss Level ");

		survivalOption.setFillColor(Color(230, 230, 230));
		campaignOption.setFillColor(Color(230, 230, 230));
		bossLevelOption.setFillColor(Color(80, 220, 255));
	}
}


void ModeSelectState::handleInput(Game& game)
{
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		game.close();
		return;
	}

	bool upKey = Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W);
	bool downKey = Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S);
	bool selectKey = Keyboard::isKeyPressed(Keyboard::Return);

	if (upKey && !previousUpKey)
	{
		selectedOption -= 1;
		if (selectedOption < 0)
		{
			selectedOption = 2;
		}
		showingCampaignMessage = false;
		game.getAudioManager().playSound(SFX_MENU_MOVE);
		refreshOptions();
	}
	else if (downKey && !previousDownKey)
	{
		selectedOption += 1;
		if (selectedOption > 2)
		{
			selectedOption = 0;
		}
		showingCampaignMessage = false;
		game.getAudioManager().playSound(SFX_MENU_MOVE);
		refreshOptions();
	}
	else if (selectKey && !previousSelectKey)
	{
		game.getAudioManager().playSound(SFX_MENU_SELECT);
		startSelectedMode(game);
	}

	previousUpKey = upKey;
	previousDownKey = downKey;
	previousSelectKey = selectKey;
}

void ModeSelectState::startSelectedMode(Game& game)
{
	if (selectedOption == 0)
	{
		game.getEntityManager().clear();
		game.changeState(new PlayState(PLAY_MODE_SURVIVAL));
		return;
	}

	if (selectedOption == 1)
	{
		game.getEntityManager().clear();
		game.changeState(new PlayState(PLAY_MODE_CAMPAIGN, 0));
		return;
	}

	if (selectedOption == 2)
	{
		game.getEntityManager().clear();
		game.changeState(new PlayState(PLAY_MODE_SURVIVAL, 0, 4));
	}
}

void ModeSelectState::update(Game& game, float deltaTime)
{
	(void)deltaTime;
	game.getAudioManager().playMusic(MUSIC_MENU);
}

void ModeSelectState::draw(Game& game, RenderWindow& window)
{
	(void)game;

	window.draw(backgroundSprite);
	window.draw(title);
	window.draw(survivalOption);
	window.draw(campaignOption);
	window.draw(bossLevelOption);
	window.draw(instructions);

	if (showingCampaignMessage)
	{
		window.draw(message);
	}
}