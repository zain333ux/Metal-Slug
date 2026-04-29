#include "Entity.h"

Entity::Entity()
{
	x = 0.0f;
	y = 0.0f;
	width = 0.0f;
	height = 0.0f;
	velocityX = 0.0f;
	velocityY = 0.0f;
	active = true;
	visible = true;
}

Entity::~Entity()
{
}

void Entity::update(float deltaTime)
{
	x += velocityX * deltaTime;
	y += velocityY * deltaTime;
	sprite.setPosition(x, y);
}

void Entity::draw(sf::RenderWindow& window)
{
	if (visible)
	{
		window.draw(sprite);
	}
}

void Entity::setPosition(float newX, float newY)
{
	x = newX;
	y = newY;
	sprite.setPosition(x, y);
}

void Entity::setSize(float newWidth, float newHeight)
{
	width = newWidth;
	height = newHeight;
}

void Entity::setVelocity(float newVelocityX, float newVelocityY)
{
	velocityX = newVelocityX;
	velocityY = newVelocityY;
}

void Entity::setSprite(const sf::Sprite& newSprite)
{
	sprite = newSprite;
}

void Entity::deactivate()
{
	active = false;
}

bool Entity::isActive() const
{
	return active;
}

bool Entity::isVisible() const
{
	return visible;
}

float Entity::getX() const
{
	return x;
}

float Entity::getY() const
{
	return y;
}

float Entity::getWidth() const
{
	return width;
}

float Entity::getHeight() const
{
	return height;
}

float Entity::getCenterX() const
{
	return x + width / 2.0f;
}

float Entity::getCenterY() const
{
	return y + height / 2.0f;
}

sf::FloatRect Entity::getBounds() const
{
	if (width > 0.0f && height > 0.0f)
	{
		return sf::FloatRect(x, y, width, height);
	}

	return sprite.getGlobalBounds();
}
