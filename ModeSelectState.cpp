#include <SFML/Graphics.hpp>
#include "ModeSelectState.h"
#include "Game.h"
#include "Level.h"
#include "PlayState.h"
using namespace sf;

ModeSelectState::ModeSelectState()
{
	selectedOption = 0;
	showingCampaignMessage = false;

	font.loadFromFile("Fonts/PressStart2P.ttf");

	backgroundTexture.loadFromFile("Sprites/Clean/Game_Select_BG.jpg");
	backgroundSprite.setTexture(backgroundTexture);

	menuMusic.openFromFile("Audio/menu_music.ogg");
	menuMusic.setLoop(true);
	menuMusic.setVolume(35.0f);
	menuMusic.play();

	moveSoundBuffer.loadFromFile("Audio/menu_move.wav");
	moveSound.setBuffer(moveSoundBuffer);
	moveSound.setVolume(65.0f);


	setupScreen();
	refreshOptions();
}

void ModeSelectState::setupScreen()
{
	title.setFont(font);
	title.setString("METAL SLUG");
	title.setCharacterSize(70);
	title.setFillColor(Color(245,245,245));
	title.setPosition(480.0f, 180.0f);

	survivalOption.setFont(font);
	survivalOption.setCharacterSize(36);
	survivalOption.setFillColor(Color(80, 220, 255));
	survivalOption.setPosition(540.0f, 360.0f);

	campaignOption.setFont(font);
	campaignOption.setCharacterSize(36);
	campaignOption.setFillColor(Color(230, 230, 230));
	campaignOption.setPosition(540.0f, 450.0f);

	instructions.setFont(font);
	instructions.setString("Use Up and Down. Press Enter.");
	instructions.setCharacterSize(24);
	instructions.setFillColor(Color(210, 220, 230));
	instructions.setPosition(540.0f, 560.0f);

	message.setFont(font);
	message.setString("Campaign profile: generated plains.");
	message.setCharacterSize(24);
	message.setFillColor(Color(255, 120, 120));
	message.setPosition(540.0f, 610.0f);
}

void ModeSelectState::playMoveSound()
{
	moveSound.play();

}

void ModeSelectState::refreshOptions()
{
	if (selectedOption == 0)
	{
		survivalOption.setString("> Survival Mode");
		campaignOption.setString("  Campaign Mode");

		survivalOption.setFillColor(sf::Color(80, 220, 255));
		campaignOption.setFillColor(sf::Color(230, 230, 230));
	}
	else
	{
		survivalOption.setString("  Survival Mode");
		campaignOption.setString("> Campaign Mode");

		survivalOption.setFillColor(sf::Color(230, 230, 230));
		campaignOption.setFillColor(sf::Color(80, 220, 255));
	}
}


void ModeSelectState::handleInput(Game& game)
{
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		game.close();
		return;
	}

	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
	{
		selectedOption = 0;
		showingCampaignMessage = false;
		playMoveSound();
		refreshOptions();
		return;
	}

	if (Keyboard::isKeyPressed(sf::Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
	{
		selectedOption = 1;
		showingCampaignMessage = false;
		playMoveSound();
		refreshOptions();
		return;
	}

	if (Keyboard::isKeyPressed(Keyboard::Return))
	{
		startSelectedMode(game);
		return;
	}
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
	}
}

void ModeSelectState::update(Game& game, float deltaTime)
{
	(void)game;
	(void)deltaTime;
}

void ModeSelectState::draw(Game& game, RenderWindow& window)
{
	(void)game;

	window.draw(backgroundSprite);
	window.draw(title);
	window.draw(survivalOption);
	window.draw(campaignOption);
	window.draw(instructions);

	if (showingCampaignMessage)
	{
		window.draw(message);
	}
}
