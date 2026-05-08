#pragma once

#include "Projectile.h"

class EnemyGrenadeProjectile : public Projectile
{
private:
	bool exploded;
	float explosionTimer;
	int currentFrame;
	float frameTimer;
	float explosionCenterX;
	float explosionCenterY;
	sf::Sprite explosionSprite;

	void explode();
	void centerExplosionSprite();

public:
	EnemyGrenadeProjectile(float startX, float startY, float throwVelocityX, float throwVelocityY);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	void onCollision();
};
