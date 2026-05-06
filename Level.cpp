#include "Level.h"

#include "Constants.h"

#include <cmath>

Level::Level(int newLevelNumber, float newWorldWidth, bool newCampaignGenerated)
{
	loaded = false;
	backgroundLoaded = false;
	blocksLoaded = false;
	levelNumber = newLevelNumber;
	campaignGenerated = newCampaignGenerated;
	worldWidth = newWorldWidth;
	if (worldWidth <= 0.0f)
	{
		worldWidth = Constants::WORLD_WIDTH_LEVEL_1;
	}
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

	if (!campaignGenerated)
	{
		buildSurvivalPlatforms();
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
		backgroundSprite.setScale(worldWidth / static_cast<float>(backgroundTexture.getSize().x), 1.0f);
		window.draw(backgroundSprite);
	}

	if (blocksLoaded)
	{
		if (campaignGenerated)
		{
			drawGeneratedTerrain(window);
		}
		else
		{
			drawTileRow(window, grassTexture, Constants::GROUND_Y);
			drawTileRow(window, dirtTexture, Constants::GROUND_Y + Constants::TILE_SIZE);
			drawTileRow(window, stoneTexture, Constants::GROUND_Y + Constants::TILE_SIZE * 2);
		}

		for (int i = 0; i < platforms.getSize(); i += 1)
		{
			drawPlatform(window, platforms.get(i));
		}
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

	for (int x = 0; x < static_cast<int>(worldWidth); x += Constants::TILE_SIZE)
	{
		tile.setPosition(static_cast<float>(x), static_cast<float>(y));
		window.draw(tile);
	}
}

void Level::drawGeneratedTerrain(sf::RenderWindow& window)
{
	sf::RectangleShape column;
	column.setSize(sf::Vector2f(static_cast<float>(Constants::TILE_SIZE), 1.0f));

	for (int x = 0; x < static_cast<int>(worldWidth); x += Constants::TILE_SIZE)
	{
		float surfaceY = getGeneratedSurfaceY(static_cast<float>(x));
		column.setPosition(static_cast<float>(x), surfaceY);
		column.setSize(sf::Vector2f(static_cast<float>(Constants::TILE_SIZE), static_cast<float>(Constants::SCREEN_HEIGHT) - surfaceY));
		column.setFillColor(sf::Color(68, 88, 64));
		window.draw(column);

		sf::Sprite grass;
		grass.setTexture(grassTexture);
		sf::Vector2u textureSize = grassTexture.getSize();
		if (textureSize.x > 0 && textureSize.y > 0)
		{
			grass.setScale(static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.x),
				static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.y));
		}
		grass.setPosition(static_cast<float>(x), surfaceY);
		window.draw(grass);
	}
}

void Level::drawPlatform(sf::RenderWindow& window, const sf::FloatRect& platform)
{
	sf::RectangleShape back;
	back.setPosition(platform.left, platform.top);
	back.setSize(sf::Vector2f(platform.width, platform.height));
	back.setFillColor(sf::Color(60, 48, 34));
	back.setOutlineColor(sf::Color(20, 20, 20));
	back.setOutlineThickness(2.0f);
	window.draw(back);

	if (!blocksLoaded)
	{
		return;
	}

	sf::Sprite tile;
	tile.setTexture(grassTexture);
	sf::Vector2u textureSize = grassTexture.getSize();
	if (textureSize.x > 0 && textureSize.y > 0)
	{
		tile.setScale(static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.x),
			static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(textureSize.y));
	}

	for (int x = static_cast<int>(platform.left); x < static_cast<int>(platform.left + platform.width); x += Constants::TILE_SIZE)
	{
		tile.setPosition(static_cast<float>(x), platform.top);
		window.draw(tile);
	}
}

void Level::buildSurvivalPlatforms()
{
	if (levelNumber == 1)
	{
		platforms.pushBack(sf::FloatRect(520.0f, 690.0f, 420.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(1080.0f, 650.0f, 380.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(1640.0f, 615.0f, 420.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(2260.0f, 655.0f, 440.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(2920.0f, 700.0f, 420.0f, 34.0f));
		return;
	}

	if (levelNumber == 2)
	{
		platforms.pushBack(sf::FloatRect(460.0f, 700.0f, 360.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(960.0f, 660.0f, 340.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(1460.0f, 620.0f, 360.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(2040.0f, 665.0f, 420.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(2680.0f, 615.0f, 380.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(3340.0f, 675.0f, 460.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(3920.0f, 705.0f, 360.0f, 34.0f));
		return;
	}

	if (levelNumber == 3)
	{
		platforms.pushBack(sf::FloatRect(460.0f, 705.0f, 340.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(900.0f, 660.0f, 320.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(1340.0f, 620.0f, 340.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(1820.0f, 575.0f, 340.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(2340.0f, 625.0f, 380.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(2920.0f, 680.0f, 420.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(3540.0f, 635.0f, 360.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(4140.0f, 690.0f, 420.0f, 34.0f));
		platforms.pushBack(sf::FloatRect(4700.0f, 710.0f, 360.0f, 34.0f));
		return;
	}

	platforms.pushBack(sf::FloatRect(520.0f, 690.0f, 420.0f, 34.0f));
	platforms.pushBack(sf::FloatRect(1220.0f, 650.0f, 420.0f, 34.0f));
	platforms.pushBack(sf::FloatRect(1880.0f, 700.0f, 420.0f, 34.0f));
}

void Level::extendIfNeeded(float playerX)
{
	if (!campaignGenerated)
	{
		return;
	}

	if (playerX > worldWidth - 1800.0f)
	{
		worldWidth += 2800.0f;
	}
}

bool Level::isLoaded() const
{
	return loaded;
}

float Level::getWorldWidth() const
{
	return worldWidth;
}

float Level::getRightBoundary() const
{
	return worldWidth - 120.0f;
}

int Level::getLevelNumber() const
{
	return levelNumber;
}

float Level::getLandingY(float left, float right, float previousBottom, float currentBottom) const
{
	float landingY = getMainGroundYAt((left + right) * 0.5f);
	for (int i = 0; i < platforms.getSize(); i += 1)
	{
		const sf::FloatRect& platform = platforms.get(i);
		bool overlapsX = right > platform.left + 6.0f && left < platform.left + platform.width - 6.0f;
		bool crossedTop = previousBottom <= platform.top + 24.0f && currentBottom >= platform.top;
		if (overlapsX && crossedTop && platform.top < landingY)
		{
			landingY = platform.top;
		}
	}

	return landingY;
}

float Level::getGroundYAt(float x) const
{
	float ground = getMainGroundYAt(x);
	for (int i = 0; i < platforms.getSize(); i += 1)
	{
		const sf::FloatRect& platform = platforms.get(i);
		if (x >= platform.left && x <= platform.left + platform.width && platform.top < ground)
		{
			ground = platform.top;
		}
	}

	return ground;
}

float Level::getMainGroundYAt(float x) const
{
	if (campaignGenerated)
	{
		return getGeneratedSurfaceY(x);
	}

	return static_cast<float>(Constants::GROUND_Y);
}

float Level::getGeneratedSurfaceY(float x) const
{
	float wave = std::sin(x * 0.006f) * 60.0f + std::sin(x * 0.017f) * 24.0f + std::sin(x * 0.011f) * 18.0f;
	float y = static_cast<float>(Constants::GROUND_Y) - 65.0f + wave;
	if (y < 500.0f)
	{
		y = 500.0f;
	}
	if (y > static_cast<float>(Constants::GROUND_Y))
	{
		y = static_cast<float>(Constants::GROUND_Y);
	}
	return y;
}
