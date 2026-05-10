#pragma once

#include "Entity.h"

class PlayerSoldier;
class Level;

enum CollectibleKind
{
	COLLECTIBLE_FRUIT,
	COLLECTIBLE_TURKEY,
	COLLECTIBLE_ROCKET_ITEM,
	COLLECTIBLE_HMG_ITEM,
	COLLECTIBLE_SUPPLY_CRATE
};

class Collectible : public Entity
{
private:
	CollectibleKind kind;
	Level* activeLevel;
	sf::Texture texture;
	sf::Sprite itemSprite;
	bool spriteLoaded;
	float lifeTime;
	sf::RectangleShape fallbackBody;
	const sf::IntRect* animationFrames;
	int animationFrameCount;
	int animationFrameIndex;
	float animationTimer;
	float animationFrameDuration;

public:
	Collectible(CollectibleKind newKind, float newX, float newY);

	void update(float deltaTime);
	void apply(PlayerSoldier& player);
	void draw(sf::RenderWindow& window);
	void setActiveLevel(Level* level);
	CollectibleKind getKind() const;

private:
	bool loadMaskedTexture(const char* fileName);
	void updateVisualPosition();
};
