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
	bool isGroundMaskPixel(sf::Color pixel) const;
	int getSourceXFromWorldX(float worldX) const;
	float getWorldYFromSourceY(int sourceY) const;
	float getMaskedGroundYAt(float x) const;
	float getMaskedLandingY(float left, float right, float previousBottom, float currentBottom) const;
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
	float getWorldHeight() const;
	float getRightBoundary() const;
	int getLevelNumber() const;
	float getLandingY(float left, float right, float previousBottom, float currentBottom) const;
	float getGroundYAt(float x) const;
	float getMainGroundYAt(float x) const;
	float getGeneratedSurfaceY(float x) const;
};
