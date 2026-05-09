#pragma once

#include "Projectile.h"

class RocketProjectile : public Projectile
{
private:
	bool exploded;
	int currentFrame;
	float frameTimer;
	float explosionCenterX;
	float explosionCenterY;
	sf::Sprite explosionSprite;

	void explode();
	void centerExplosionSprite();

public:
	RocketProjectile(float startX, float startY, bool facingRight, bool airborne);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	void onCollision();
};
