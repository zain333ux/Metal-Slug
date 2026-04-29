#include "Projectile.h"

Projectile::Projectile()
{
	damage = 10;
	lifeTime = 2.0f;
	width = 22.0f;
	height = 8.0f;

	body.setSize(sf::Vector2f(width, height));
	body.setFillColor(sf::Color(255, 230, 90));
	body.setOutlineColor(sf::Color::Black);
	body.setOutlineThickness(1.0f);
}

void Projectile::update(float deltaTime)
{
	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f)
	{
		deactivate();
		return;
	}

	Entity::update(deltaTime);
	body.setPosition(x, y);
}

void Projectile::draw(sf::RenderWindow& window)
{
	if (visible)
	{
		window.draw(body);
	}
}

int Projectile::getDamage() const
{
	return damage;
}
