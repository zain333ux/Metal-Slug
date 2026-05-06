#pragma once

#include <SFML/Graphics.hpp>

#include "DynamicArray.h"

class Level
{
protected:
	bool loaded;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	bool backgroundLoaded;
	sf::Texture grassTexture;
	sf::Texture dirtTexture;
	sf::Texture stoneTexture;
	bool blocksLoaded;
	float worldWidth;
	int levelNumber;
	bool campaignGenerated;
	DynamicArray<sf::FloatRect> platforms;

	void drawTileRow(sf::RenderWindow& window, sf::Texture& texture, int y);
	void drawPlatform(sf::RenderWindow& window, const sf::FloatRect& platform);
	void drawGeneratedTerrain(sf::RenderWindow& window);
	void buildSurvivalPlatforms();

public:
	Level(int newLevelNumber = 1, float newWorldWidth = 0.0f, bool newCampaignGenerated = false);
	virtual ~Level();

	virtual void load();
	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);
	void extendIfNeeded(float playerX);

	bool isLoaded() const;
	float getWorldWidth() const;
	float getRightBoundary() const;
	int getLevelNumber() const;
	float getLandingY(float left, float right, float previousBottom, float currentBottom) const;
	float getGroundYAt(float x) const;
	float getMainGroundYAt(float x) const;
	float getGeneratedSurfaceY(float x) const;
};
