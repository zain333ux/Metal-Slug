#include "Entity.h"


using namespace std;
using namespace sf;

Entity::Entity()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	velocityX = 0;
	velocityY = 0;
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

void Entity::draw(RenderWindow& window)
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

<<<<<<< HEAD
void Entity::setSprite(const Sprite& newSprite)
{
	sprite = newSprite;
}

=======
>>>>>>> 6d77c54b1816a0bae93ea8c1ad344f5e135692cd
void Entity::deactivate()
{
	active = false;
}

bool Entity::isActive() const
{
	return active;
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
	return x + width / 2;
}

float Entity::getCenterY() const
{
	return y + height / 2;
}

FloatRect Entity::getBounds() const
{
	if (width > 0 && height > 0)
	{
		return FloatRect(x, y, width, height);
	}

	return sprite.getGlobalBounds();
}