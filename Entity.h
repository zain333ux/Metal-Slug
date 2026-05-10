#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
protected:
	float x;
	float y;
	float width;
	float height;
	float velocityX;
	float velocityY;
	bool active;
	bool visible;
	sf::Sprite sprite;

public:
	Entity();
	virtual ~Entity();

	virtual void update(float deltaTime);
	virtual void draw(sf::RenderWindow& window);

	void setPosition(float newX, float newY);
	void setSize(float newWidth, float newHeight);
	void setVelocity(float newVelocityX, float newVelocityY);
	void deactivate();

	bool isActive() const;
	float getX() const;
	float getY() const;
	float getWidth() const;
	float getHeight() const;
	float getCenterX() const;
	float getCenterY() const;
	sf::FloatRect getBounds() const;
};
