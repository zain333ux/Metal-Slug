#include "Game.h"
#include "Constants.h"
#include "ModeSelectState.h"


using namespace std;
using namespace sf;


Game::Game(): window(VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "Metal Slug OOP", Style::Close)
{
	running = false;
}

void Game::initialize()
{
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(Constants::FRAME_LIMIT);
	audioManager.loadAll();
	entityManager.setAudioManager(&audioManager);

	gameStateManager.changeState(new ModeSelectState());
	running = true;
}


void Game::run()
{
	initialize();

	Clock clock;
	while (running && window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();
		processEvents();
		update(deltaTime);
		render();
	}
}

void Game::close()
{
	running = false;
	window.close();
}

void Game::changeState(GameState* newState)
{
	gameStateManager.changeState(newState);
}

void Game::processEvents()
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			close();
		}
	}

	gameStateManager.handleInput(*this);
}

void Game::update(float deltaTime)
{
	gameStateManager.update(*this, deltaTime);
}

void Game::render()
{
	window.clear(Color(30, 35, 40));
	gameStateManager.draw(*this, window);
	window.display();
}

EntityManager& Game::getEntityManager()
{
	return entityManager;
}

LevelManager& Game::getLevelManager()
{
	return levelManager;
}

AudioManager& Game::getAudioManager()
{
	return audioManager;
}

RenderWindow& Game::getWindow()
{
	return window;
}