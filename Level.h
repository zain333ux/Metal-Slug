#pragma once

#include <SFML/Graphics.hpp>

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

	void drawTileRow(sf::RenderWindow& window, sf::Texture& texture, int y);

public:
	Level();
	virtual ~Level();

	virtual void load();
	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);

	bool isLoaded() const;
};
