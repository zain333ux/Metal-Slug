#include "Collectible.h"

#include "PlayerSoldier.h"

Collectible::Collectible(CollectibleKind newKind, float newX, float newY)
{
	kind = newKind;
	width = 42.0f;
	height = 42.0f;
	setPosition(newX, newY);

	body.setSize(sf::Vector2f(width, height));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(2.0f);

	if (kind == COLLECTIBLE_SUPPLY_CRATE)
	{
		body.setFillColor(sf::Color(170, 105, 45));
	}
	else if (kind == COLLECTIBLE_POW)
	{
		body.setFillColor(sf::Color(240, 240, 245));
	}
	else
	{
		body.setFillColor(sf::Color(245, 90, 80));
	}
}

void Collectible::apply(PlayerSoldier& player)
{
	if (kind == COLLECTIBLE_SUPPLY_CRATE || kind == COLLECTIBLE_POW)
	{
		player.refillDemoInventory();
	}
	else
	{
		player.heal(25);
	}

	deactivate();
}

void Collectible::draw(sf::RenderWindow& window)
{
	if (visible)
	{
		body.setPosition(x, y);
		window.draw(body);
	}
}

CollectibleKind Collectible::getKind() const
{
	return kind;
}
