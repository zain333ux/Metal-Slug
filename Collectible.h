#pragma once

#include "Entity.h"

class PlayerSoldier;

enum CollectibleKind
{
	COLLECTIBLE_SUPPLY_CRATE,
	COLLECTIBLE_FOOD,
	COLLECTIBLE_POW
};

class Collectible : public Entity
{
private:
	CollectibleKind kind;
	sf::RectangleShape body;

public:
	Collectible(CollectibleKind newKind, float newX, float newY);

	void apply(PlayerSoldier& player);
	void draw(sf::RenderWindow& window);
	CollectibleKind getKind() const;
};
