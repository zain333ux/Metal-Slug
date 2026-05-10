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
	bool fullBiomeBackground;
	sf::IntRect backgroundTextureRect;
	sf::Image collisionMaskImage;
	bool collisionMaskLoaded;
	DynamicArray<float> groundProfile;
	DynamicArray<sf::FloatRect> waterBlocks;
	int groundProfileSpacing;
	sf::Texture grassTexture;
	sf::Texture dirtTexture;
	sf::Texture stoneTexture;
	bool blocksLoaded;
	float worldWidth;
	float worldHeight;
	int levelNumber;
	bool campaignGenerated;
	DynamicArray<sf::FloatRect> platforms;

	bool loadBackground();
	bool loadFullBiomeBackground(const char* fileName, const char* collisionMaskFileName);
	bool loadCollisionMask(const char* fileName);
	void setupFullBiomeRect(int contentTop, int contentHeight);
	void findImageContentY(const sf::Image& image, int& contentTop, int& contentHeight) const;
	void buildGroundProfileFromMask();
	void buildWaterBlocksFromMask();
	bool isGroundMaskPixel(sf::Color pixel) const;
	bool isWaterMaskPixel(sf::Color pixel) const;
	int getSourceXFromWorldX(float worldX) const;
	int getSourceYFromWorldY(float worldY) const;
	float getWorldYFromSourceY(int sourceY) const;
	float getMaskedGroundYAt(float x) const;
	float getMaskedLandingY(float left, float right, float previousBottom, float currentBottom) const;
	bool hasWaterInRect(const sf::FloatRect& rect) const;
	void drawTileRow(sf::RenderWindow& window, sf::Texture& texture, int y);
	void drawPlatform(sf::RenderWindow& window, const sf::FloatRect& platform);
	void drawGeneratedTerrain(sf::RenderWindow& window);
	void drawWaterBlocks(sf::RenderWindow& window);
	void buildSurvivalPlatforms();

public:
	Level(int newLevelNumber = 1, float newWorldWidth = 0.0f, bool newCampaignGenerated = false);
	virtual ~Level() = 0;

	virtual void load();
	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);
	void extendIfNeeded(float playerX);

	float getWorldWidth() const;
	float getWorldHeight() const;
	float getRightBoundary() const;
	float getLandingY(float left, float right, float previousBottom, float currentBottom) const;
	float getGroundYAt(float x) const;
	float getMainGroundYAt(float x) const;
	float getGeneratedSurfaceY(float x) const;
	char getCollisionTileAt(float worldX, float worldY) const;
	bool isWaterInBounds(float left, float right, float top, float bottom) const;
	float getWaterSurfaceYAt(float x) const;
	bool isAerialBiome(float x, float y) const;
	bool isPlainsBiome(float x, float y) const;
	bool isAquaticBiome(float x, float y) const;
};

class GameLevel : public Level
{
public:
	GameLevel(int newLevelNumber = 1, float newWorldWidth = 0, bool newCampaignGenerated = false);
};
