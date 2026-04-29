#include "Level.h"

#include "Constants.h"

Level::Level()
{
	loaded = false;
	backgroundLoaded = false;
	blocksLoaded = false;
}

Level::~Level()
{
}

void Level::load()
{
	if (backgroundTexture.loadFromFile("Sprites/Clean/background_level1.png"))
	{
		backgroundSprite.setTexture(backgroundTexture);
		backgroundLoaded = true;
	}

	if (grassTexture.loadFromFile("Sprites/blocks/grass_block_side.png") &&
		dirtTexture.loadFromFile("Sprites/blocks/dirt.png") &&
		stoneTexture.loadFromFile("Sprites/blocks/stone.png"))
	{
		blocksLoaded = true;
	}

	loaded = true;
}

void Level::update(float deltaTime)
{
	(void)deltaTime;
}

void Level::draw(sf::RenderWindow& window)
{
	if (backgroundLoaded)
	{
		window.draw(backgroundSprite);
	}

	if (blocksLoaded)
	{
		drawTileRow(window, grassTexture, Constants::GROUND_Y);
		drawTileRow(window, dirtTexture, Constants::GROUND_Y + Constants::TILE_SIZE);
		drawTileRow(window, stoneTexture, Constants::GROUND_Y + Constants::TILE_SIZE * 2);
	}
	else
	{
		sf::RectangleShape ground;
		ground.setPosition(0.0f, static_cast<float>(Constants::GROUND_Y));
		ground.setSize(sf::Vector2f(static_cast<float>(Constants::SCREEN_WIDTH), static_cast<float>(Constants::SCREEN_HEIGHT - Constants::GROUND_Y)));
		ground.setFillColor(sf::Color(90, 70, 45, 120));
		window.draw(ground);
	}
}

void Level::drawTileRow(sf::RenderWindow& window, sf::Texture& texture, int y)
{
	sf::Sprite tile;
	tile.setTexture(texture);

	sf::Vector2u textureSize = texture.getSize();
	if (textureSize.x > 0 && textureSize.y > 0)
	{
		float scaleX = static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.x);
		float scaleY = static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.y);
		tile.setScale(scaleX, scaleY);
	}

	for (int x = 0; x < Constants::SCREEN_WIDTH; x += Constants::TILE_SIZE)
	{
		tile.setPosition(static_cast<float>(x), static_cast<float>(y));
		window.draw(tile);
	}
}

bool Level::isLoaded() const
{
	return loaded;
}
