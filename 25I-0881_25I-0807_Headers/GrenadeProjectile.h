#pragma once

#include "BallisticProjectile.h"

class GrenadeProjectile : public BallisticProjectile
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
	GrenadeProjectile(float startX, float startY, bool facingRight, bool airborne);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	void onCollision();
};
